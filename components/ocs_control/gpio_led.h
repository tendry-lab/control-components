/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_control/basic_led.h"
#include "ocs_core/noncopyable.h"
#include "ocs_io/gpio/igpio.h"

namespace ocs {
namespace control {

class GpioLED : public BasicLED, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p gpio to control LED.
    explicit GpioLED(io::gpio::IGpio& gpio);

    //! Turn on LED.
    status::StatusCode turn_on() override;

    //! Turn off LED.
    status::StatusCode turn_off() override;

    //! Flip LED state.
    status::StatusCode flip() override;

private:
    io::gpio::IGpio& gpio_;
};

} // namespace control
} // namespace ocs
