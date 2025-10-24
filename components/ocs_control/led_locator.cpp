/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <algorithm>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_control/led_locator.h"
#include "ocs_core/lock_guard.h"
#include "ocs_core/log.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace control {

namespace {

const char* log_tag = "led_locator";

} // namespace

LedLocator::LedLocator(scheduler::ITaskScheduler& task_scheduler,
                       scheduler::AsyncFuncScheduler& func_scheduler,
                       ILed& led)
    : task_scheduler_(task_scheduler)
    , func_scheduler_(func_scheduler)
    , led_(led) {
}

status::StatusCode LedLocator::turn_on() {
    core::LockGuard lock(mu_);

    if (enabled_) {
        return status::StatusCode::OK;
    }

    return turn_on_();
}

status::StatusCode LedLocator::turn_off() {
    core::LockGuard lock(mu_);

    if (!enabled_) {
        return status::StatusCode::OK;
    }

    return turn_off_();
}

status::StatusCode LedLocator::flip() {
    core::LockGuard lock(mu_);

    return enabled_ ? turn_off_() : turn_on_();
}

bool LedLocator::get() const {
    core::LockGuard lock(mu_);

    return enabled_;
}

status::StatusCode LedLocator::run() {
    const auto code = led_.try_lock(ILed::Priority::Locate);
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to lock LED on locating: %s",
                 status::code_to_str(code));

        return code;
    }

    configASSERT(led_.flip() == status::StatusCode::OK);

    return status::StatusCode::OK;
}

status::StatusCode LedLocator::turn_on_() {
    auto future = func_scheduler_.add([this]() {
        return handle_turn_on_();
    });
    if (!future) {
        return status::StatusCode::InvalidState;
    }
    if (future->wait(wait_timeout_) != status::StatusCode::OK) {
        return status::StatusCode::Timeout;
    }
    if (future->code() != status::StatusCode::OK) {
        return future->code();
    }

    enabled_ = true;

    return status::StatusCode::OK;
}

status::StatusCode LedLocator::turn_off_() {
    auto future = func_scheduler_.add([this]() {
        return handle_turn_off_();
    });
    if (!future) {
        return status::StatusCode::InvalidState;
    }
    if (future->wait(wait_timeout_) != status::StatusCode::OK) {
        return status::StatusCode::Timeout;
    }

    if (future->code() != status::StatusCode::OK) {
        return future->code();
    }

    enabled_ = false;

    return status::StatusCode::OK;
}

status::StatusCode LedLocator::handle_turn_on_() {
    const auto code = led_.try_lock(ILed::Priority::Locate);
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to lock LED on locating enabling: %s",
                 status::code_to_str(code));

        return code;
    }

    configASSERT(led_.turn_off() == status::StatusCode::OK);

    configASSERT(task_scheduler_.add(*this, task_id_, task_interval_)
                 == status::StatusCode::OK);

    return status::StatusCode::OK;
}

status::StatusCode LedLocator::handle_turn_off_() {
    const auto code = led_.try_lock(ILed::Priority::Locate);
    if (code == status::StatusCode::OK) {
        configASSERT(led_.turn_off() == status::StatusCode::OK);
        configASSERT(led_.try_unlock(ILed::Priority::Locate) == status::StatusCode::OK);
    } else {
        ocs_logw(log_tag, "failed to lock LED on locating disabling: %s",
                 status::code_to_str(code));
    }

    configASSERT(task_scheduler_.remove(task_id_) == status::StatusCode::OK);

    return status::StatusCode::OK;
}

} // namespace control
} // namespace ocs
