/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_core/static_mutex.h"
#include "ocs_status/macros.h"

namespace ocs {
namespace core {

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
