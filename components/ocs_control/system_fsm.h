/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>

#include "ocs_control/ibutton.h"
#include "ocs_control/ibutton_handler.h"
#include "ocs_control/ifsr_handler.h"
#include "ocs_control/iled.h"
#include "ocs_core/noncopyable.h"
#include "ocs_core/static_event_group.h"
#include "ocs_scheduler/ievent_handler.h"
#include "ocs_scheduler/itask.h"
#include "ocs_scheduler/itask_scheduler.h"
#include "ocs_system/iclock.h"
#include "ocs_system/irebooter.h"

namespace ocs {
namespace control {

class SystemFsm : public IButtonHandler,
                  public scheduler::ITask,
                  private scheduler::IEventHandler,
                  private core::NonCopyable<> {
public:
    struct Params {
        //! How long to wait for the FSR to be started.
        system::Time fsr_wait_begin_interval { system::Duration::second * 10 };

        //! How long to wait for the FSR to be confirmed.
        system::Time fsr_wait_confirm_interval { system::Duration::second * 20 };

        //! How to long to wait for the button to be released to initiate the reboot.
        system::Time release_interval { system::Duration::second * 4 };
    };

    //! Initialize.
    //!
    //! @params
    //!  - @p rebooter to be called when the button is pressed (ignored during the FSR).
    //!  - @p clock to measure the FSR state durations.
    //!  - @p task_scheduler to schedule the LED reactions.
    //!  - @p init_handler to be called when the system initialization is completed.
    //!  - @p fsr_handler to be called when the FSR is happened.
    //!  - @p led to signalize about the system events.
    //!  - @p button to check if the FSR is requested.
    //!  - @p params - various configuration parameters.
    SystemFsm(system::IRebooter& rebooter,
              system::IClock& clock,
              scheduler::ITaskScheduler& task_scheduler,
              scheduler::IEventHandler& init_handler,
              IFsrHandler& fsr_handler,
              ILed& led,
              IButton& button,
              Params params);

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
        FsrWaitBegin,
        FsrWaitConfirm,
        FsrDone,
    };

    //! Handle LED blinked required number of times.
    status::StatusCode handle_event() override;

    void handle_init_done_();

    void handle_state_init_();
    void handle_state_wait_reset_(bool pressed);

    void handle_state_fsr_wait_begin_();
    void handle_state_fsr_wait_confirm_(bool pressed);
    void handle_state_fsr_done_();

    void add_fsr_task_();
    void add_led_task_(unsigned flip_count);
    void remove_task_();

    bool button_was_pressed_();

    static constexpr system::Time task_interval_ = system::Duration::second / 2;
    static constexpr unsigned flip_count_init_ = 6;
    static constexpr unsigned flip_count_button_pressed_ = 2;
    static constexpr const char* task_id_ = "sys_fsm_led";

    const Params params_;

    system::IRebooter& rebooter_;
    system::IClock& clock_;
    scheduler::ITaskScheduler& task_scheduler_;
    scheduler::IEventHandler& init_handler_;
    IFsrHandler& fsr_handler_;
    ILed& led_;
    IButton& button_;

    std::unique_ptr<scheduler::ITask> task_;

    core::StaticEventGroup event_group_;
    State state_ { State::Init };
    system::Time fsr_update_ts_ { 0 };
};

} // namespace control
} // namespace ocs
