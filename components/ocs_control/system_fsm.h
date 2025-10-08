/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>

#include "ocs_control/ibutton_handler.h"
#include "ocs_control/iled.h"
#include "ocs_core/noncopyable.h"
#include "ocs_core/static_event_group.h"
#include "ocs_scheduler/ievent_handler.h"
#include "ocs_scheduler/itask.h"
#include "ocs_scheduler/itask_scheduler.h"
#include "ocs_system/irebooter.h"

namespace ocs {
namespace control {

class SystemFsm : public IButtonHandler,
                  public scheduler::ITask,
                  private scheduler::IEventHandler,
                  private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p rebooter to be called when the button is pressed (ignored during the FSR).
    //!  - @p task_scheduler to schedule the LED reactions.
    //!  - @p led to signalize about the system events.
    //!  - @p release_interval - ensure button is released within the provided interval.
    SystemFsm(system::IRebooter& rebooter,
              scheduler::ITaskScheduler& task_scheduler,
              ILED& led,
              system::Time release_interval);

    //! Handle asynchronous button events.
    status::StatusCode run() override;

    //! Dispatch asynchronous button events.
    //!
    //! @remarks
    //!  ISR safe.
    status::StatusCode handle_pressed(system::Time duration) override;

private:
    enum class State : uint8_t {
        Init,
        WaitInitDone,
        WaitReset,
        WaitResetDone,
    };

    //! Handle LED blinked required number of times.
    status::StatusCode handle_event() override;

    void handle_state_init_();
    void handle_state_wait_reset_();

    void add_led_task_(unsigned flip_count);
    void remove_led_task_();

    bool button_pressed_();

    static constexpr system::Time led_task_interval_ = system::Duration::second / 2;
    static constexpr unsigned led_flip_count_init_ = 6;
    static constexpr unsigned led_flip_count_button_pressed_ = 2;
    static constexpr const char* led_task_id_ = "sys_fsm_led";

    const system::Time release_interval_ { 0 };

    system::IRebooter& rebooter_;
    scheduler::ITaskScheduler& task_scheduler_;
    ILED& led_;

    std::unique_ptr<scheduler::ITask> led_task_;

    core::StaticEventGroup event_group_;
    State state_ { State::Init };
};

} // namespace control
} // namespace ocs
