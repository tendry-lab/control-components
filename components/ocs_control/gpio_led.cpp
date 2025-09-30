/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_control/gpio_led.h"

namespace ocs {
namespace control {

GpioLED::GpioLED(io::gpio::IGpio& gpio)
    : gpio_(gpio) {
}

status::StatusCode GpioLED::turn_on() {
    if (!inuse_) {
        return status::StatusCode::InvalidState;
    }

    return gpio_.turn_on();
}

status::StatusCode GpioLED::turn_off() {
    if (!inuse_) {
        return status::StatusCode::InvalidState;
    }

    return gpio_.turn_off();
}

status::StatusCode GpioLED::flip() {
    if (!inuse_) {
        return status::StatusCode::InvalidState;
    }

    return gpio_.flip();
}

} // namespace control
} // namespace ocs
