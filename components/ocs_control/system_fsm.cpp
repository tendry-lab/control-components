/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_control/system_fsm.h"
#include "ocs_control/led_task.h"

#define EVENT_BITS_BUTTON_PRESSED BIT0
#define EVENT_BITS_ALL (EVENT_BITS_BUTTON_PRESSED)

namespace ocs {
namespace control {

SystemFsm::SystemFsm(system::IRebooter& rebooter,
                     scheduler::ITaskScheduler& task_scheduler,
                     ILED& led,
                     core::Time release_interval)
    : release_interval_(release_interval)
    , rebooter_(rebooter)
    , task_scheduler_(task_scheduler)
    , led_(led) {
}

status::StatusCode SystemFsm::run() {
    switch (state_) {
    case State::Init:
        handle_state_init_();
        break;

    case State::WaitReset:
        handle_state_wait_reset_();
        break;

    case State::WaitInitDone:
    case State::WaitResetDone:
        break;
    }

    return status::StatusCode::OK;
}

status::StatusCode SystemFsm::handle_pressed(core::Time duration) {
    if (duration > release_interval_) {
        return status::StatusCode::Error;
    }

    BaseType_t task_woken = pdFALSE;

    xEventGroupSetBitsFromISR(event_group_.get(), EVENT_BITS_BUTTON_PRESSED, &task_woken);
    if (task_woken) {
        portYIELD_FROM_ISR();
    }

    return status::StatusCode::OK;
}

status::StatusCode SystemFsm::handle_event() {
    remove_led_task_();

    switch (state_) {
    case State::WaitInitDone:
        state_ = State::WaitReset;
        break;

    case State::WaitResetDone:
        rebooter_.reboot();
        break;

    default:
        //! Invalid state.
        configASSERT(false);
        break;
    }

    return status::StatusCode::OK;
}

void SystemFsm::handle_state_init_() {
    add_led_task_(led_flip_count_init_);

    state_ = State::WaitInitDone;
}

void SystemFsm::handle_state_wait_reset_() {
    if (button_pressed_()) {
        add_led_task_(led_flip_count_button_pressed_);

        state_ = State::WaitResetDone;
    }
}

void SystemFsm::add_led_task_(unsigned flip_count) {
    configASSERT(!led_task_);

    led_task_.reset(new (std::nothrow)
                        LEDTask(*this, led_, ILED::Priority::System, flip_count));
    configASSERT(led_task_);

    configASSERT(led_.try_lock(ILED::Priority::System) == status::StatusCode::OK);
    configASSERT(led_.turn_off() == status::StatusCode::OK);

    configASSERT(task_scheduler_.add(*led_task_, led_task_id_, led_task_interval_)
                 == status::StatusCode::OK);
}

void SystemFsm::remove_led_task_() {
    configASSERT(led_task_);

    configASSERT(task_scheduler_.remove(led_task_id_) == status::StatusCode::OK);

    led_task_ = nullptr;

    configASSERT(led_.unlock() == status::StatusCode::OK);
}

bool SystemFsm::button_pressed_() {
    const EventBits_t bits =
        xEventGroupWaitBits(event_group_.get(), EVENT_BITS_ALL, pdTRUE, pdFALSE, 0);

    return bits & EVENT_BITS_BUTTON_PRESSED;
}

} // namespace control
} // namespace ocs
