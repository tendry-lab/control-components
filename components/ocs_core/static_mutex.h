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

//! FreeRTOS mutex to be used in FreeRTOS tasks.
class StaticMutex : public ILocker, private NonCopyable<> {
public:
    //! Initialize.
    StaticMutex();

    //! Lock the mutex.
    status::StatusCode lock(TickType_t wait = portMAX_DELAY) override;

    //! Unlock the mutex.
    status::StatusCode unlock() override;

private:
    SemaphoreHandle_t sem_ { nullptr };
    StaticSemaphore_t buff_;
};

} // namespace core
} // namespace ocs
