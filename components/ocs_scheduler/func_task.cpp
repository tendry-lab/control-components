/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_scheduler/func_task.h"

namespace ocs {
namespace scheduler {

FuncTask::FuncTask(Func func)
    : func_(func) {
}

ocs::status::StatusCode FuncTask::run() {
    return func_();
}

} // namespace scheduler
} // namespace ocs
