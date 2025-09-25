/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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

LEDLocator::LEDLocator(scheduler::ITaskScheduler& task_scheduler,
                       scheduler::AsyncFuncScheduler& func_scheduler)
    : task_scheduler_(task_scheduler)
    , func_scheduler_(func_scheduler) {
}

void LEDLocator::add(ILED& led) {
    const auto it = std::find_if(leds_.begin(), leds_.end(), [&led](const auto& l) {
        return l == &led;
    });
    configASSERT(it == leds_.end());

    leds_.push_back(&led);
}

status::StatusCode LEDLocator::turn_on() {
    configASSERT(leds_.size());

    core::LockGuard lock(mu_);

    if (enabled_) {
        return status::StatusCode::OK;
    }

    return turn_on_();
}

status::StatusCode LEDLocator::turn_off() {
    configASSERT(leds_.size());

    core::LockGuard lock(mu_);

    if (!enabled_) {
        return status::StatusCode::OK;
    }

    return turn_off_();
}

status::StatusCode LEDLocator::flip() {
    configASSERT(leds_.size());

    core::LockGuard lock(mu_);

    return enabled_ ? turn_off_() : turn_on_();
}

bool LEDLocator::get() const {
    core::LockGuard lock(mu_);

    return enabled_;
}

status::StatusCode LEDLocator::run() {
    for (unsigned n = 0; n < leds_.size(); ++n) {
        const auto code = leds_[n]->flip();
        if (code != status::StatusCode::OK) {
            ocs_loge(log_tag, "failed to flip LED on task run(): %s",
                     status::code_to_str(code));

            return code;
        }
    }

    return status::StatusCode::OK;
}

status::StatusCode LEDLocator::turn_on_() {
    auto code = add_task_();
    if (code == status::StatusCode::OK) {
        code = lock_leds_();
    }
    if (code != status::StatusCode::OK) {
        if (const auto c = remove_task_(); c != status::StatusCode::OK) {
            ocs_loge(log_tag, "failed to remove task on turn_on(): %s",
                     status::code_to_str(c));
        }

        return code;
    }

    enabled_ = true;

    return status::StatusCode::OK;
}

status::StatusCode LEDLocator::turn_off_() {
    auto code = remove_task_();
    if (code == status::StatusCode::OK) {
        code = unlock_leds_();
    }
    if (code != status::StatusCode::OK) {
        return code;
    }

    enabled_ = false;

    return status::StatusCode::OK;
}

status::StatusCode LEDLocator::add_task_() {
    auto future = func_scheduler_.add([this]() {
        return task_scheduler_.add(*this, task_id_, task_interval_);
    });
    if (!future) {
        return status::StatusCode::InvalidState;
    }
    if (future->wait(wait_timeout_) != status::StatusCode::OK) {
        return status::StatusCode::Timeout;
    }

    return future->code();
}

status::StatusCode LEDLocator::remove_task_() {
    auto future = func_scheduler_.add([this]() {
        return task_scheduler_.remove(task_id_);
    });
    if (!future) {
        return status::StatusCode::InvalidState;
    }
    if (future->wait(wait_timeout_) != status::StatusCode::OK) {
        return status::StatusCode::Timeout;
    }

    return future->code();
}

status::StatusCode LEDLocator::lock_leds_() {
    auto future = func_scheduler_.add([this]() {
        for (unsigned n = 0; n < leds_.size(); ++n) {
            auto code = leds_[n]->unlock();
            if (code != status::StatusCode::OK) {
                ocs_loge(log_tag,
                         "failed to unlock LED on locating enabling: pos=%u code=%s", n,
                         status::code_to_str(code));

                return code;
            }

            code = leds_[n]->lock();
            if (code != status::StatusCode::OK) {
                ocs_loge(log_tag,
                         "failed to lock LED on locating enabling: pos=%u code=%s", n,
                         status::code_to_str(code));

                return code;
            }

            code = leds_[n]->turn_off();
            if (code != status::StatusCode::OK) {
                ocs_loge(log_tag,
                         "failed to turn off LED on locating enabling: pos=%u code=%s", n,
                         status::code_to_str(code));

                return code;
            }
        }

        return status::StatusCode::OK;
    });
    if (!future) {
        return status::StatusCode::InvalidState;
    }
    if (future->wait(wait_timeout_) != status::StatusCode::OK) {
        return status::StatusCode::Timeout;
    }

    return future->code();
}

status::StatusCode LEDLocator::unlock_leds_() {
    auto future = func_scheduler_.add([this]() {
        for (unsigned n = 0; n < leds_.size(); ++n) {
            auto code = leds_[n]->turn_off();
            if (code != status::StatusCode::OK) {
                ocs_loge(log_tag,
                         "failed to turn off LED on locating disabling: pos=%u code=%s",
                         n, status::code_to_str(code));

                return code;
            }

            code = leds_[n]->unlock();
            if (code != status::StatusCode::OK) {
                ocs_loge(log_tag,
                         "failed to unlock LED on locating disabling: pos=%u code=%s", n,
                         status::code_to_str(code));

                return code;
            }
        }

        return status::StatusCode::OK;
    });
    if (!future) {
        return status::StatusCode::InvalidState;
    }
    if (future->wait(wait_timeout_) != status::StatusCode::OK) {
        return status::StatusCode::Timeout;
    }

    return future->code();
}

} // namespace control
} // namespace ocs
