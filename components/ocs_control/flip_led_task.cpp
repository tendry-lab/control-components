/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_control/flip_led_task.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace control {

FlipLedTask::FlipLedTask(scheduler::IEventHandler& handler,
                         ILed& led,
                         ILed::Priority priority,
                         size_t flip_count)
    : priority_(priority)
    , flip_count_(flip_count)
    , handler_(handler)
    , led_(led) {
}

status::StatusCode FlipLedTask::run() {
    configASSERT(flip_count_);

    auto code = led_.try_lock(priority_);
    if (code != status::StatusCode::OK) {
        return code;
    }

    code = led_.flip();
    if (code != status::StatusCode::OK) {
        return code;
    }

    --flip_count_;
    if (!flip_count_) {
        return handler_.handle_event();
    }

    return status::StatusCode::OK;
}

} // namespace control
} // namespace ocs
