/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_core/static_mutex_isr.h"
#include "ocs_status/macros.h"

namespace ocs {
namespace core {

status::StatusCode StaticMutexIsr::lock(TickType_t) {
    BaseType_t high_task_woken = pdFALSE;

    OCS_STATUS_RETURN_ON_FALSE(xSemaphoreTakeFromISR(sem_, &high_task_woken) == pdTRUE,
                               status::StatusCode::Error);

    OCS_STATUS_RETURN_ON_FALSE(high_task_woken == pdTRUE, status::StatusCode::OK);

    return status::StatusCode::IsrYield;
}

status::StatusCode StaticMutexIsr::unlock() {
    BaseType_t high_task_woken = pdFALSE;

    OCS_STATUS_RETURN_ON_FALSE(xSemaphoreGiveFromISR(sem_, &high_task_woken) == pdTRUE,
                               status::StatusCode::Error);

    OCS_STATUS_RETURN_ON_FALSE(high_task_woken == pdTRUE, status::StatusCode::OK);

    return status::StatusCode::IsrYield;
}

} // namespace core
} // namespace ocs
