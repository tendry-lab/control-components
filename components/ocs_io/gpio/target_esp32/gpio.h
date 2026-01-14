/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_io/gpio/igpio.h"
#include "ocs_io/gpio/types.h"

namespace ocs {
namespace io {
namespace gpio {

class Gpio : public IGpio, public core::NonCopyable<Gpio> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p gpio_num - GPIO number.
    //!  - @p enable_level - which level should be used to activate the GPIO.
    Gpio(GpioNum gpio_num, Level enable_level);

    //! Destroy.
    virtual ~Gpio() = default;

    //! Get the GPIO level.
    status::StatusCode get_level(Level& level) override;

    //! Change GPIO state to opposite.
    status::StatusCode flip() override;

    //! Enable GPIO.
    status::StatusCode turn_on() override;

    //! Disable GPIO.
    status::StatusCode turn_off() override;

    //! Set the GPIO direction.
    status::StatusCode set_direction(Direction direction) override;

private:
    const GpioNum gpio_num_ { static_cast<io::gpio::GpioNum>(-1) };
    const Level enable_level_ { 0 };
};

} // namespace gpio
} // namespace io
} // namespace ocs
