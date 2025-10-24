/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "freertos/FreeRTOS.h"

#include "ocs_core/ilocker.h"
#include "ocs_core/noncopyable.h"

namespace ocs {
namespace core {

//! RAII resource locker.
class LockGuard : private NonCopyable<> {
public:
    //! Acquire the resource.
    //!
    //! @params
    //!  - @p locker to lock/unlock the resource.
    //!  - @p wait - interval to wait for the resource to become available.
    LockGuard(ILocker& locker, TickType_t wait = portMAX_DELAY);

    //! Release the resource.
    ~LockGuard();

private:
    ILocker& locker_;
};

} // namespace core
} // namespace ocs
