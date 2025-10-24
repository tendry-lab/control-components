/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "freertos/FreeRTOS.h"

#include "ocs_control/iled.h"
#include "ocs_control/ilocator.h"
#include "ocs_core/noncopyable.h"
#include "ocs_core/static_mutex.h"
#include "ocs_scheduler/async_func_scheduler.h"
#include "ocs_scheduler/itask_scheduler.h"
#include "ocs_system/time.h"

namespace ocs {
namespace control {

class LedLocator : public ILocator,
                   private scheduler::ITask,
                   private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p task_scheduler to execute the locating procedure.
    //!  - @p func_scheduler to schedule the locating procedure.
    //!  - @p led to periodically flip during the locating procedure.
    //!
    //! @remarks
    //!  All operations on the LEDs should be scheduled on the same task scheduler.
    LedLocator(scheduler::ITaskScheduler& task_scheduler,
               scheduler::AsyncFuncScheduler& func_scheduler,
               ILed& led);

    //! Enable device locating.
    //!
    //! @remarks
    //!  Can be called from multiple tasks.
    status::StatusCode turn_on() override;

    //! Disable device locating.
    //!
    //! @remarks
    //!  Can be called from multiple tasks.
    status::StatusCode turn_off() override;

    //! Flip device locating
    //!
    //! @remarks
    //!  Can be called from multiple tasks.
    status::StatusCode flip() override;

    //! Return true if device locating is enabled.
    //!
    //! @remarks
    //!  Can be called from multiple tasks.
    bool get() const override;

private:
    status::StatusCode run() override;

    status::StatusCode turn_on_();
    status::StatusCode turn_off_();
    status::StatusCode handle_turn_on_();
    status::StatusCode handle_turn_off_();

    static constexpr const char* task_id_ = "led_locator_task";
    static constexpr TickType_t wait_timeout_ = pdMS_TO_TICKS(1000 * 5);
    static constexpr system::Time task_interval_ = system::Duration::second / 2;

    scheduler::ITaskScheduler& task_scheduler_;
    scheduler::AsyncFuncScheduler& func_scheduler_;

    ILed& led_;

    mutable core::StaticMutex mu_;
    bool enabled_ { false };
};

} // namespace control
} // namespace ocs
