/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_scheduler/operation_guard_task.h"
#include "ocs_core/operation_guard.h"

namespace ocs {
namespace scheduler {

OperationGuardTask::OperationGuardTask(ITask& task)
    : task_(task) {
}

status::StatusCode OperationGuardTask::run() {
    core::OperationGuard operation_guard;

    return task_.run();
}

} // namespace scheduler
} // namespace ocs
