/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_control/gpio_button.h"
#include "ocs_control/ibutton.h"

namespace ocs {
namespace control {

GpioButton::GpioButton(io::gpio::IGpio& gpio, io::gpio::Level level)
    : level_(level)
    , gpio_(gpio) {
}

status::StatusCode GpioButton::get_pressed(bool& pressed) {
    io::gpio::Level level = 0;

    const auto code = gpio_.get_level(level);
    if (code != ocs::status::StatusCode::OK) {
        return code;
    }

    pressed = (level == level_);

    return status::StatusCode::OK;
}

} // namespace control
} // namespace ocs
