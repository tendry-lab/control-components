/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_control/gpio_button.h"
#include "ocs_control/ibutton.h"

namespace ocs {
namespace control {

GpioButton::GpioButton(io::gpio::IGpio& gpio, int level)
    : level_(level)
    , gpio_(gpio) {
}

bool GpioButton::get() {
    return gpio_.get() == level_;
}

} // namespace control
} // namespace ocs
