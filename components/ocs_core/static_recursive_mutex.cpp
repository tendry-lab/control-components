/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_core/static_recursive_mutex.h"
#include "ocs_status/macros.h"

namespace ocs {
namespace core {

status::StatusCode StaticRecursiveMutex::lock(TickType_t wait) {
    OCS_STATUS_RETURN_ON_FALSE(xSemaphoreTakeRecursive(sem_, wait) == pdTRUE,
                               status::StatusCode::Error);

    return status::StatusCode::OK;
}

status::StatusCode StaticRecursiveMutex::unlock() {
    OCS_STATUS_RETURN_ON_FALSE(xSemaphoreGiveRecursive(sem_) == pdTRUE,
                               status::StatusCode::Error);

    return status::StatusCode::OK;
}

} // namespace core
} // namespace ocs
