/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_scheduler/async_isr_task.h"

namespace ocs {
namespace scheduler {

AsyncIsrTask::AsyncIsrTask(EventGroupHandle_t handle, EventBits_t event)
    : event_(event)
    , handle_(handle) {
}

status::StatusCode AsyncIsrTask::run() {
    BaseType_t task_woken = pdFALSE;

    xEventGroupSetBitsFromISR(handle_, event_, &task_woken);
    if (task_woken) {
        portYIELD_FROM_ISR();
    }

    return status::StatusCode::OK;
}

} // namespace scheduler
} // namespace ocs
