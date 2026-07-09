/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstring>

#include "ocs_algo/bit_ops.h"
#include "ocs_core/freertos.h"
#include "ocs_core/log.h"
#include "ocs_scheduler/async_task.h"
#include "ocs_scheduler/async_task_scheduler.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace scheduler {

AsyncTaskScheduler::AsyncTaskScheduler(system::IArena& arena,
                                       system::ITimerBuilder& timer_builder,
                                       IDelayEstimator& estimator,
                                       const char* id)
    : log_tag_(id)
    , arena_(arena)
    , timer_builder_(timer_builder)
    , estimator_(estimator) {
}

size_t AsyncTaskScheduler::max_count() const {
    //!  8 high bits are used by the FreeRTOS itself.
    return (sizeof(EventBits_t) * 8) - 8;
}

status::StatusCode
AsyncTaskScheduler::add(ITask& task, const char* id, system::Time interval) {
    configASSERT(id);
    configASSERT(interval > 0);
    configASSERT(interval >= system::Duration::microsecond);

    EventBits_t event = 0;

    const auto code = allocate_event_(event, id);
    if (code != status::StatusCode::OK) {
        return code;
    }

    auto node = ocs::system::make_shared_ptr<Node>(
        arena_, arena_, timer_builder_, task, event_group_.get(), event, interval, id);
    configASSERT(node);

    nodes_.emplace_back(node);

    return status::StatusCode::OK;
}

status::StatusCode AsyncTaskScheduler::remove(const char*) {
    return status::StatusCode::NotSupported;
}

status::StatusCode AsyncTaskScheduler::start() {
    ocs_logi(log_tag_.c_str(), "start tasks scheduling: count=%u/%u", nodes_.size(),
             max_count());

    for (auto& node : nodes_) {
        if (node->get_clock_type() == Node::ClockType::External) {
            continue;
        }

        const auto code = node->run();
        if (code != status::StatusCode::OK) {
            ocs_logw(log_tag_.c_str(),
                     "failed to run node on scheduler start: id=%s code=%s", node->id(),
                     status::code_to_str(code));
        }
    }

    for (auto& node : nodes_) {
        if (node->get_clock_type() == Node::ClockType::External) {
            continue;
        }

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
        if (node->get_clock_type() == Node::ClockType::External) {
            continue;
        }

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

EventGroupHandle_t AsyncTaskScheduler::get_event_group() {
    return event_group_.get();
}

status::StatusCode
AsyncTaskScheduler::attach(EventBits_t& event, ITask& task, const char* id) {
    const auto code = allocate_event_(event, id);
    if (code != status::StatusCode::OK) {
        return code;
    }

    auto node = ocs::system::make_shared_ptr<Node>(arena_, task, event, id,
                                                   Node::ClockType::External);
    configASSERT(node);

    nodes_.emplace_back(node);

    return status::StatusCode::OK;
}

status::StatusCode AsyncTaskScheduler::allocate_event_(EventBits_t& event,
                                                       const char* id) {
    if (nodes_.size() == max_count()) {
        return status::StatusCode::Error;
    }

    for (auto& node : nodes_) {
        if (strcmp(node->id(), id) == 0) {
            return status::StatusCode::InvalidArg;
        }
    }

    event = algo::BitOps::mask_u32(nodes_.size());

    bits_all_ |= event;

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
                               EventBits_t event,
                               const char* id,
                               AsyncTaskScheduler::Node::ClockType clock_type)
    : id_(id)
    , event_(event)
    , clock_type_(clock_type)
    , task_(task) {
}

AsyncTaskScheduler::Node::Node(system::IArena& arena,
                               system::ITimerBuilder& timer_builder,
                               ITask& task,
                               EventGroupHandle_t even_group,
                               EventBits_t event,
                               system::Time interval,
                               const char* id)
    : Node(task, event, id, ClockType::Internal) {
    async_task_ = ocs::system::make_unique_ptr<AsyncTask>(arena, even_group, event);
    configASSERT(async_task_);

    timer_ = timer_builder.make_timer(*async_task_, id, interval);
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

AsyncTaskScheduler::Node::ClockType AsyncTaskScheduler::Node::get_clock_type() const {
    return clock_type_;
}

status::StatusCode AsyncTaskScheduler::Node::start() {
    return timer_->start();
}

status::StatusCode AsyncTaskScheduler::Node::stop() {
    return timer_->stop();
}

} // namespace scheduler
} // namespace ocs
