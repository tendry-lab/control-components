/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "ocs_core/ilocker.h"
#include "ocs_core/noncopyable.h"

namespace ocs {
namespace core {

//! Static FreeRTOS recursive mutex.
class BasicStaticRecursiveMutex : public ILocker,
                                  public NonCopyable<BasicStaticRecursiveMutex> {
public:
    //! Initialize.
    BasicStaticRecursiveMutex();

    //! Destroy.
    virtual ~BasicStaticRecursiveMutex() = default;

protected:
    SemaphoreHandle_t sem_ { nullptr };

private:
    StaticSemaphore_t buff_;
};

} // namespace core
} // namespace ocs
