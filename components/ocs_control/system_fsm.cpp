/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_control/system_fsm.h"
#include "ocs_algo/time_ops.h"
#include "ocs_control/flip_led_task.h"
#include "ocs_core/log.h"
#include "ocs_status/code_to_str.h"

#define EVENT_BITS_BUTTON_PRESSED BIT0
#define EVENT_BITS_ALL (EVENT_BITS_BUTTON_PRESSED)

namespace ocs {
namespace control {

namespace {

class FsrLedTask : public scheduler::ITask, private core::NonCopyable<> {
public:
    FsrLedTask(ILed& led, ILed::Priority priority)
        : priority_(priority)
        , led_(led) {
    }

    status::StatusCode run() override {
        auto code = led_.try_lock(priority_);
        if (code != status::StatusCode::OK) {
            return code;
        }

        code = led_.flip();
        if (code != status::StatusCode::OK) {
            return code;
        }

        return status::StatusCode::OK;
    }

private:
    const ILed::Priority priority_ { ILed::Priority::None };

    ILed& led_;
};

const char* log_tag = "system_fsm";

} // namespace

SystemFsm::SystemFsm(system::IRebooter& rebooter,
                     system::IClock& clock,
                     scheduler::ITaskScheduler& task_scheduler,
                     scheduler::IEventHandler& init_handler,
                     IFsrHandler& fsr_handler,
                     ILed& led,
                     IButton& button,
                     Params params)
    : params_(params)
    , rebooter_(rebooter)
    , clock_(clock)
    , task_scheduler_(task_scheduler)
    , init_handler_(init_handler)
    , fsr_handler_(fsr_handler)
    , led_(led)
    , button_(button) {
}

status::StatusCode SystemFsm::run() {
    const auto button_was_pressed = button_was_pressed_();

    switch (state_) {
    case State::Init:
        handle_state_init_();
        break;

    case State::WaitReset:
        handle_state_wait_reset_(button_was_pressed);
        break;

    case State::FsrWaitBegin:
        handle_state_fsr_wait_begin_();
        break;

    case State::FsrWaitConfirm:
        handle_state_fsr_wait_confirm_(button_was_pressed);
        break;

    case State::FsrDone:
        handle_state_fsr_done_();
        break;

    case State::WaitInitDone:
    case State::WaitResetDone:
        break;
    }

    return status::StatusCode::OK;
}

status::StatusCode SystemFsm::handle_pressed(system::Time duration) {
    if (duration > params_.release_interval) {
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
    remove_task_();

    switch (state_) {
    case State::WaitInitDone:
        handle_init_done_();
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

void SystemFsm::handle_init_done_() {
    const auto code = init_handler_.handle_event();
    if (code != status::StatusCode::OK) {
        ocs_logw(log_tag, "failed to handle initialization completion event");
    }

    state_ = State::WaitReset;
}

void SystemFsm::handle_state_init_() {
    if (!button_.get()) {
        add_led_task_(flip_count_init_);
        state_ = State::WaitInitDone;

        return;
    }

    configASSERT(!fsr_update_ts_);

    fsr_update_ts_ = clock_.now();
    state_ = State::FsrWaitBegin;

    ocs_logi(log_tag, "FSR requested: wait begin");
}

void SystemFsm::handle_state_wait_reset_(bool was_pressed) {
    if (was_pressed) {
        add_led_task_(flip_count_button_pressed_);
        state_ = State::WaitResetDone;
    }
}

void SystemFsm::handle_state_fsr_wait_begin_() {
    configASSERT(fsr_update_ts_);

    if (!button_.get()) {
        ocs_logw(log_tag, "FSR canceled: button released too quickly");

        fsr_update_ts_ = 0;

        handle_init_done_();

        return;
    }

    const auto now = clock_.now();

    if (!algo::TimeOps::after(fsr_update_ts_, now, params_.fsr_wait_begin_interval)) {
        return;
    }

    add_fsr_task_();

    fsr_update_ts_ = now;
    state_ = State::FsrWaitConfirm;

    ocs_logi(log_tag, "FSR started: wait confirmation");
}

void SystemFsm::handle_state_fsr_wait_confirm_(bool was_pressed) {
    configASSERT(fsr_update_ts_);

    const auto expired = algo::TimeOps::after(fsr_update_ts_, clock_.now(),
                                              params_.fsr_wait_confirm_interval);
    if (expired) {
        ocs_logw(log_tag, "FSR canceled: isn't confirmed within timeout");

        remove_task_();

        fsr_update_ts_ = 0;

        handle_init_done_();

        return;
    }

    if (!was_pressed) {
        return;
    }

    const auto code = fsr_handler_.handle_fsr();
    if (code != status::StatusCode::OK) {
        ocs_logw(log_tag, "FSR handler failed: code=%s", status::code_to_str(code));
    }

    remove_task_();

    fsr_update_ts_ = 0;
    state_ = State::FsrDone;

    ocs_logi(log_tag, "FSR completed");
}

void SystemFsm::handle_state_fsr_done_() {
    add_led_task_(flip_count_button_pressed_);
    state_ = State::WaitResetDone;
}

void SystemFsm::add_fsr_task_() {
    configASSERT(!task_);

    task_.reset(new (std::nothrow) FsrLedTask(led_, ILed::Priority::System));
    configASSERT(task_);

    configASSERT(led_.try_lock(ILed::Priority::System) == status::StatusCode::OK);
    configASSERT(led_.turn_off() == status::StatusCode::OK);

    configASSERT(task_scheduler_.add(*task_, task_id_, task_interval_)
                 == status::StatusCode::OK);
}

void SystemFsm::add_led_task_(size_t flip_count) {
    configASSERT(!task_);

    task_.reset(new (std::nothrow)
                    FlipLedTask(*this, led_, ILed::Priority::System, flip_count));
    configASSERT(task_);

    configASSERT(led_.try_lock(ILed::Priority::System) == status::StatusCode::OK);
    configASSERT(led_.turn_off() == status::StatusCode::OK);

    configASSERT(task_scheduler_.add(*task_, task_id_, task_interval_)
                 == status::StatusCode::OK);
}

void SystemFsm::remove_task_() {
    configASSERT(task_);

    configASSERT(task_scheduler_.remove(task_id_) == status::StatusCode::OK);
    task_ = nullptr;

    configASSERT(led_.try_lock(ILed::Priority::System) == status::StatusCode::OK);
    configASSERT(led_.turn_off() == status::StatusCode::OK);
    configASSERT(led_.try_unlock(ILed::Priority::System) == status::StatusCode::OK);
}

bool SystemFsm::button_was_pressed_() {
    const EventBits_t bits =
        xEventGroupWaitBits(event_group_.get(), EVENT_BITS_ALL, pdTRUE, pdFALSE, 0);

    return bits & EVENT_BITS_BUTTON_PRESSED;
}

} // namespace control
} // namespace ocs
