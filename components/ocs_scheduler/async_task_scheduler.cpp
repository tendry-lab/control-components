/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_algo/bit_ops.h"
#include "ocs_core/log.h"
#include "ocs_scheduler/async_task.h"
#include "ocs_scheduler/async_task_scheduler.h"
#include "ocs_status/code_to_str.h"
#include "ocs_system/platform_builder.h"

namespace ocs {
namespace scheduler {

AsyncTaskScheduler::AsyncTaskScheduler(IDelayEstimator& estimator, const char* id)
    : log_tag_(id)
    , estimator_(estimator) {
}

unsigned AsyncTaskScheduler::max_count() const {
    //!  8 high bits are used by the FreeRTOS itself.
    return (sizeof(EventBits_t) * 8) - 8;
}

status::StatusCode
AsyncTaskScheduler::add(ITask& task, const char* id, core::Time interval) {
    configASSERT(id);
    configASSERT(interval > 0);
    configASSERT(interval >= core::Duration::microsecond);

    if (nodes_.size() == max_count()) {
        return status::StatusCode::Error;
    }

    for (auto& node : nodes_) {
        if (strcmp(node->id(), id) == 0) {
            return status::StatusCode::InvalidArg;
        }
    }

    const EventBits_t event = algo::BitOps::mask(nodes_.size());

    NodePtr node(new (std::nothrow) Node(task, event_group_.get(), event, interval, id));
    configASSERT(node);

    nodes_.emplace_back(node);

    bits_all_ |= event;

    return status::StatusCode::OK;
}

status::StatusCode AsyncTaskScheduler::remove(const char* id) {
    return status::StatusCode::NotSupported;
}

status::StatusCode AsyncTaskScheduler::start() {
    ocs_logi(log_tag_.c_str(), "start tasks scheduling: count=%u/%u", nodes_.size(),
             max_count());

    for (auto& node : nodes_) {
        const auto code = node->run();
        if (code != status::StatusCode::OK) {
            ocs_logw(log_tag_.c_str(),
                     "failed to run node on scheduler start: id=%s code=%s", node->id(),
                     status::code_to_str(code));
        }
    }

    for (auto& node : nodes_) {
        const auto code = node->start();
        if (code != status::StatusCode::OK) {
            ocs_logw(log_tag_.c_str(),
                     "failed to start node on scheduler start: id=%s code=%s", node->id(),
                     status::code_to_str(code));
        }
    }

    return status::StatusCode::OK;
}

status::StatusCode AsyncTaskScheduler::stop() {
    ocs_logi(log_tag_.c_str(), "stop tasks scheduling: count=%u/%u", nodes_.size(),
             max_count());

    for (auto& node : nodes_) {
        const auto code = node->stop();
        if (code != status::StatusCode::OK) {
            ocs_loge(log_tag_.c_str(), "failed to stop node: id=%s code=%s", node->id(),
                     status::code_to_str(code));
        }
    }

    return status::StatusCode::OK;
}

status::StatusCode AsyncTaskScheduler::run() {
    estimator_.begin();
    const auto delay = estimator_.estimate();

    const EventBits_t bits =
        xEventGroupWaitBits(event_group_.get(), bits_all_, pdTRUE, pdFALSE, delay);

    run_(bits);

    return status::StatusCode::OK;
}

void AsyncTaskScheduler::run_(EventBits_t bits) {
    for (auto& node : nodes_) {
        if (bits & node->event()) {
            const auto code = node->run();
            if (code != status::StatusCode::OK) {
                ocs_loge(log_tag_.c_str(), "failed to run task: id=%s code=%s",
                         node->id(), status::code_to_str(code));
            }
        }
    }
}

AsyncTaskScheduler::Node::Node(ITask& task,
                               EventGroupHandle_t even_group,
                               EventBits_t event,
                               core::Time interval,
                               const char* id)
    : id_(id)
    , event_(event)
    , task_(task) {
    async_task_.reset(new (std::nothrow) AsyncTask(even_group, event));
    configASSERT(async_task_);

    timer_ =
        system::PlatformBuilder::make_high_resolution_timer(*async_task_, id, interval);
    configASSERT(timer_);
}

status::StatusCode AsyncTaskScheduler::Node::run() {
    return task_.run();
}

const char* AsyncTaskScheduler::Node::id() const {
    return id_.c_str();
}

EventBits_t AsyncTaskScheduler::Node::event() const {
    return event_;
}

status::StatusCode AsyncTaskScheduler::Node::start() {
    return timer_->start();
}

status::StatusCode AsyncTaskScheduler::Node::stop() {
    return timer_->stop();
}

} // namespace scheduler
} // namespace ocs
