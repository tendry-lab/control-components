/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <vector>

#include "freertos/FreeRTOS.h"

#include "ocs_control/iled.h"
#include "ocs_control/ilocator.h"
#include "ocs_core/noncopyable.h"
#include "ocs_core/static_mutex.h"
#include "ocs_core/time.h"
#include "ocs_scheduler/async_func_scheduler.h"
#include "ocs_scheduler/itask_scheduler.h"

namespace ocs {
namespace control {

class LEDLocator : public ILocator,
                   private scheduler::ITask,
                   private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p task_scheduler to execute the locating procedure.
    //!  - @p func_scheduler to schedule the locating procedure.
    //!
    //! @remarks
    //!  All operations on the LEDs should be scheduled on the same task scheduler.
    LEDLocator(scheduler::ITaskScheduler& task_scheduler,
               scheduler::AsyncFuncScheduler& func_scheduler);

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

    //! Add @p led to be used during the locating procedure.
    //!
    //! @remarks
    //!  - LED should be added only once.
    void add(ILED& led);

private:
    status::StatusCode run() override;

    status::StatusCode turn_on_();
    status::StatusCode turn_off_();
    status::StatusCode handle_turn_on_();
    status::StatusCode handle_turn_off_();

    static constexpr const char* task_id_ = "led_locator_task";
    static constexpr TickType_t wait_timeout_ = pdMS_TO_TICKS(1000 * 5);
    static constexpr core::Time task_interval_ = core::Duration::second / 2;

    scheduler::ITaskScheduler& task_scheduler_;
    scheduler::AsyncFuncScheduler& func_scheduler_;

    std::vector<ILED*> leds_;

    mutable core::StaticMutex mu_;
    bool enabled_ { false };
};

} // namespace control
} // namespace ocs
