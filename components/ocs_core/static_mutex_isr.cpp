/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_core/static_mutex_isr.h"

namespace ocs {
namespace core {

status::StatusCode StaticMutexIsr::lock(TickType_t) {
    BaseType_t high_task_woken = pdFALSE;

    if (xSemaphoreTakeFromISR(sem_, &high_task_woken) == pdFALSE) {
        return status::StatusCode::Error;
    }

    return high_task_woken == pdTRUE ? ocs::status::StatusCode::IsrYield
                                     : ocs::status::StatusCode::OK;
}

status::StatusCode StaticMutexIsr::unlock() {
    BaseType_t high_task_woken = pdFALSE;

    if (xSemaphoreGiveFromISR(sem_, &high_task_woken) == errQUEUE_FULL) {
        return status::StatusCode::Error;
    }

    return high_task_woken == pdTRUE ? ocs::status::StatusCode::IsrYield
                                     : ocs::status::StatusCode::OK;
}

} // namespace core
} // namespace ocs
