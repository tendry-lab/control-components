/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/basic_static_recursive_mutex.h"
#include "ocs_core/noncopyable.h"

namespace ocs {
namespace core {

//! FreeRTOS recursive mutex to be used in FreeRTOS tasks.
class StaticRecursiveMutex : public BasicStaticRecursiveMutex, private NonCopyable<> {
public:
    //! Lock the mutex.
    status::StatusCode lock(TickType_t wait = portMAX_DELAY) override;

    //! Unlock the mutex.
    status::StatusCode unlock() override;
};

} // namespace core
} // namespace ocs
