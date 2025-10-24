/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_control/basic_led.h"
#include "ocs_core/noncopyable.h"
#include "ocs_io/gpio/igpio.h"

namespace ocs {
namespace control {

class GpioLed : public BasicLed, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p gpio to control LED.
    explicit GpioLed(io::gpio::IGpio& gpio);

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
