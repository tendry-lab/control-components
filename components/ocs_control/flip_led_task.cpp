/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_control/flip_led_task.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace control {

FlipLedTask::FlipLedTask(ILED& led, ILED::Priority priority)
    : priority_(priority)
    , led_(led) {
}

status::StatusCode FlipLedTask::run() {
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

} // namespace control
} // namespace ocs
