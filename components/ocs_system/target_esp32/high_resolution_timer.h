/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_timer.h"

#include "ocs_core/noncopyable.h"
#include "ocs_scheduler/itask.h"
#include "ocs_system/itimer.h"
#include "ocs_system/time.h"

namespace ocs {
namespace system {

class HighResolutionTimer : public ITimer, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p task to be invoked periodically at the configured interval.
    //!  - @p name to distinguish one timer from another.
    //!  - @p interval - timer interval.
    HighResolutionTimer(scheduler::ITask& task, const char* name, system::Time interval);

    //! Destroy timer.
    ~HighResolutionTimer();

    //! Start ESP high-resolution timer.
    status::StatusCode start() override;

    //! Stop ESP high-resolution timer.
    status::StatusCode stop() override;

private:
    static void handle_cb_(void* arg);

    const system::Time interval_ { 0 };

    scheduler::ITask& task_;

    esp_timer_handle_t handle_ { nullptr };
};

} // namespace system
} // namespace ocs
