/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_scheduler/fanout_task.h"

namespace ocs {
namespace scheduler {

status::StatusCode FanoutTask::run() {
    for (auto& task : tasks_) {
        const auto code = task->run();
        if (code != status::StatusCode::OK) {
            return code;
        }
    }

    return status::StatusCode::OK;
}

void FanoutTask::add(ITask& task) {
    tasks_.emplace_back(&task);
}

} // namespace scheduler
} // namespace ocs
