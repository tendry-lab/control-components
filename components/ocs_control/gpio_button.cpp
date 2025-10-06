/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
