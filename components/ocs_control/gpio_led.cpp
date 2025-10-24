/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_control/gpio_led.h"

namespace ocs {
namespace control {

GpioLed::GpioLed(io::gpio::IGpio& gpio)
    : gpio_(gpio) {
}

status::StatusCode GpioLed::turn_on() {
    if (!inuse_) {
        return status::StatusCode::InvalidState;
    }

    return gpio_.turn_on();
}

status::StatusCode GpioLed::turn_off() {
    if (!inuse_) {
        return status::StatusCode::InvalidState;
    }

    return gpio_.turn_off();
}

status::StatusCode GpioLed::flip() {
    if (!inuse_) {
        return status::StatusCode::InvalidState;
    }

    return gpio_.flip();
}

} // namespace control
} // namespace ocs
