/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "driver/gpio.h"

#include "ocs_core/noncopyable.h"
#include "ocs_io/gpio/igpio.h"

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
    Gpio(gpio_num_t gpio_num, Level enable_level);

    //! Destroy.
    virtual ~Gpio() = default;

    //! Return GPIO number.
    GpioNum get_num() const override;

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
    const gpio_num_t gpio_num_ { GPIO_NUM_NC };
    const Level enable_level_ { 0 };
};

} // namespace gpio
} // namespace io
} // namespace ocs
