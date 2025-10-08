/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_control/basic_led.h"

namespace ocs {
namespace control {

status::StatusCode BasicLED::try_lock(ILED::Priority priority) {
    if (inuse_) {
        if (priority_ > priority) {
            return status::StatusCode::InvalidArg;
        }
    }

    priority_ = priority;
    inuse_ = true;

    return status::StatusCode::OK;
}

status::StatusCode BasicLED::try_unlock(ILED::Priority priority) {
    if (priority_ != priority) {
        return status::StatusCode::InvalidArg;
    }

    inuse_ = false;
    priority_ = ILED::Priority::None;

    return status::StatusCode::OK;
}

} // namespace control
} // namespace ocs
