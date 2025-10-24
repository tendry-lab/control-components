/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_status/code.h"

namespace ocs {
namespace io {
namespace gpio {

class IGpio {
public:
    //! Possible GPIO directions.
    enum class Direction : uint8_t {
        Output,
        Input,
    };

    //! Destroy.
    virtual ~IGpio() = default;

    //! Get the GPIO level.
    virtual int get() = 0;

    //! Change the GPIO state to the opposite.
    virtual status::StatusCode flip() = 0;

    //! Enable the GPIO.
    virtual status::StatusCode turn_on() = 0;

    //! Disable the GPIO.
    virtual status::StatusCode turn_off() = 0;

    //! Set the GPIO direction.
    //!
    //! @notes
    //!  In most cases the GPIO direction is set once when the GPIOs are configured.
    //!  For some setups it is necessary to change the GPIO direction, e.g. 1-Wire sensors
    //!  require the GPIO direction to be changed depending on whether the data is
    //!  receiving/transmitting from/to the sensor.
    virtual status::StatusCode set_direction(Direction direction) = 0;
};

} // namespace gpio
} // namespace io
} // namespace ocs
