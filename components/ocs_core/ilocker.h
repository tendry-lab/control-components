/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "freertos/FreeRTOS.h"

#include "ocs_status/code.h"

namespace ocs {
namespace core {

//! Resource locker/unlocker.
class ILocker {
public:
    //! Destroy.
    virtual ~ILocker() = default;

    //! Lock the resource.
    virtual status::StatusCode lock(TickType_t wait = portMAX_DELAY) = 0;

    //! Unlock the resource.
    virtual status::StatusCode unlock() = 0;
};

} // namespace core
} // namespace ocs
