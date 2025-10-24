/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_control/basic_led.h"

namespace ocs {
namespace control {

status::StatusCode BasicLed::try_lock(ILed::Priority priority) {
    if (inuse_) {
        if (priority_ > priority) {
            return status::StatusCode::InvalidArg;
        }
    }

    priority_ = priority;
    inuse_ = true;

    return status::StatusCode::OK;
}

status::StatusCode BasicLed::try_unlock(ILed::Priority priority) {
    if (priority_ != priority) {
        return status::StatusCode::InvalidArg;
    }

    inuse_ = false;
    priority_ = ILed::Priority::None;

    return status::StatusCode::OK;
}

} // namespace control
} // namespace ocs
