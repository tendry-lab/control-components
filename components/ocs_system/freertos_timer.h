/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/freertos.h"
#include "ocs_core/noncopyable.h"
#include "ocs_scheduler/itask.h"
#include "ocs_system/itimer.h"
#include "ocs_system/time.h"

namespace ocs {
namespace system {

//! Periodic timer backed by a FreeRTOS software timer.
//!
//! @remarks
//!  Resolution is limited to one FreeRTOS tick (typically 1 ms).
//!  For microsecond-level precision use a platform-specific hardware timer.
class FreeRtosTimer : public ITimer, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p task to be invoked periodically at the configured interval.
    //!  - @p id to distinguish one timer from another.
    //!  - @p interval - timer period in microseconds (rounded to the nearest tick).
    FreeRtosTimer(scheduler::ITask& task, const char* id, system::Time interval);

    //! Delete FreeRTOS software timer.
    ~FreeRtosTimer();

    //! Start the periodic timer.
    status::StatusCode start() override;

    //! Stop the periodic timer.
    status::StatusCode stop() override;

private:
    static void handle_cb_(TimerHandle_t handle);

    scheduler::ITask& task_;

    TimerHandle_t handle_ { nullptr };
};

} // namespace system
} // namespace ocs
