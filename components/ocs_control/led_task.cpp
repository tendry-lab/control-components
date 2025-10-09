/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_control/led_task.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace control {

LEDTask::LEDTask(scheduler::IEventHandler& handler,
                 ILed& led,
                 ILed::Priority priority,
                 unsigned flip_count)
    : priority_(priority)
    , flip_count_(flip_count)
    , handler_(handler)
    , led_(led) {
}

status::StatusCode LEDTask::run() {
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
