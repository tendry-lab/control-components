/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstring>

#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/task.h"

#include "ocs_core/log.h"
#include "ocs_scheduler/periodic_task_scheduler.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace scheduler {

PeriodicTaskScheduler::PeriodicTaskScheduler(system::IClock& clock,
                                             IDelayEstimator& estimator,
                                             const char* id,
                                             unsigned max_count)
    : max_count_(max_count)
    , log_tag_(id)
    , clock_(clock)
    , estimator_(estimator) {
    configASSERT(max_count_);
}

unsigned PeriodicTaskScheduler::max_count() const {
    return max_count_;
}

status::StatusCode
PeriodicTaskScheduler::add(ITask& task, const char* id, system::Time interval) {
    configASSERT(id);
    configASSERT(interval > 0);
    configASSERT(interval >= system::Duration::millisecond);

    if (node_exist_(nodes_all_, id) || node_exist_(nodes_to_add_, id)) {
        return status::StatusCode::InvalidArg;
    }

    configASSERT(!node_exist_(nodes_to_remove_, id));

    const auto nodes_count = nodes_all_.size() + nodes_to_add_.size();
    if (nodes_count == max_count()) {
        return status::StatusCode::Error;
    }

    NodePtr node(new (std::nothrow) Node(clock_, task, id, interval));
    configASSERT(node);

    nodes_to_add_.push_back(node);

    return status::StatusCode::OK;
}

status::StatusCode PeriodicTaskScheduler::remove(const char* id) {
    configASSERT(id);

    if (node_exist_(nodes_to_remove_, id)) {
        return status::StatusCode::InvalidArg;
    }

    if (node_exist_(nodes_to_add_, id)) {
        node_remove_(nodes_to_add_, id);

        return status::StatusCode::OK;
    }

    auto node = node_exist_(nodes_all_, id);
    if (!node) {
        return status::StatusCode::InvalidArg;
    }

    node->set_alive(false);
    nodes_to_remove_.push_back(node);

    return status::StatusCode::OK;
}

status::StatusCode PeriodicTaskScheduler::start() {
    ocs_logi(log_tag_.c_str(), "start tasks scheduling: pending=%u max=%u",
             nodes_to_add_.size(), max_count());

    return status::StatusCode::OK;
}

status::StatusCode PeriodicTaskScheduler::stop() {
    ocs_logi(log_tag_.c_str(), "stop tasks scheduling");

    return status::StatusCode::OK;
}

status::StatusCode PeriodicTaskScheduler::run() {
    estimator_.begin();

    const auto start_ts = clock_.now();
    run_();
    const auto total_ts = clock_.now() - start_ts;

    total_ts_min_ = std::min(total_ts_min_, total_ts);
    total_ts_max_ = std::max(total_ts_max_, total_ts);

    const auto estimated_delay = estimator_.estimate();

    ocs_logd(log_tag_.c_str(),
             "delay estimating: total=%lli(usec) total_min=%lli(usec) "
             "total_max=%lli(usec) estimated=%lu(ms)",
             total_ts, total_ts_min_, total_ts_max_, pdTICKS_TO_MS(estimated_delay));

    vTaskDelay(estimated_delay);

    return status::StatusCode::OK;
}

PeriodicTaskScheduler::NodePtr
PeriodicTaskScheduler::node_exist_(const PeriodicTaskScheduler::NodeList& nodes,
                                   const char* id) {
    for (const auto& node : nodes) {
        if (strcmp(node->id(), id) == 0) {
            return node;
        }
    }

    return nullptr;
}

void PeriodicTaskScheduler::node_remove_(PeriodicTaskScheduler::NodeList& nodes,
                                         const char* id) {
    const auto count = std::erase_if(nodes, [id](const auto& node) {
        return strcmp(id, node->id()) == 0;
    });

    configASSERT(count == 1);
}

void PeriodicTaskScheduler::run_() {
    for (const auto& node : nodes_to_add_) {
        node->set_alive(true);
        nodes_all_.push_back(node);
    }
    nodes_to_add_.clear();

    for (auto& node : nodes_all_) {
        if (!node->alive()) {
            continue;
        }

        const auto code = node->run();
        if (code != status::StatusCode::OK) {
            ocs_loge(log_tag_.c_str(), "failed to run task: id=%s code=%s", node->id(),
                     status::code_to_str(code));
        }
    }

    for (const auto& node : nodes_to_remove_) {
        node_remove_(nodes_all_, node->id());
    }
    nodes_to_remove_.clear();
}

PeriodicTaskScheduler::Node::Node(system::IClock& clock,
                                  ITask& task,
                                  const char* id,
                                  system::Time interval)
    : id_(id)
    , task_(task)
    , limiter_(clock, interval) {
}

status::StatusCode PeriodicTaskScheduler::Node::run() {
    return limiter_.allow() ? task_.run() : status::StatusCode::OK;
}

const char* PeriodicTaskScheduler::Node::id() const {
    return id_.c_str();
}

bool PeriodicTaskScheduler::Node::alive() const {
    return alive_;
}

void PeriodicTaskScheduler::Node::set_alive(bool alive) {
    alive_ = alive;
}

} // namespace scheduler
} // namespace ocs
