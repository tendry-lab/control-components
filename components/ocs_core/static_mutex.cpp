/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/static_mutex.h"
#include "ocs_status/macros.h"

namespace ocs {
namespace core {

StaticMutex::StaticMutex() {
    memset(&buff_, 0, sizeof(buff_));

    sem_ = xSemaphoreCreateMutexStatic(&buff_);
    configASSERT(sem_);
}

status::StatusCode StaticMutex::lock(TickType_t wait) {
    OCS_STATUS_RETURN_ON_FALSE(xSemaphoreTake(sem_, wait) == pdTRUE,
                               status::StatusCode::Error);

    return status::StatusCode::OK;
}

status::StatusCode StaticMutex::unlock() {
    OCS_STATUS_RETURN_ON_FALSE(xSemaphoreGive(sem_) == pdTRUE, status::StatusCode::Error);

    return status::StatusCode::OK;
}

} // namespace core
} // namespace ocs
