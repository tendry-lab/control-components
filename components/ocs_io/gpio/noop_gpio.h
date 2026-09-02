/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_io/gpio/igpio.h"

namespace ocs {
namespace io {
namespace gpio {

class NoopGpio : public IGpio, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p gpio_num - GPIO number to be reported to the caller.
    explicit NoopGpio(GpioNum gpio_num)
        : gpio_num_(gpio_num) {
    }

    //! Return GPIO number.
    GpioNum get_num() const override {
        return gpio_num_;
    }

    //! Non-operational level getting.
    status::StatusCode get_level(Level& level) override {
        level = 0;

        return status::StatusCode::OK;
    }

    //! Non-operational level setting.
    status::StatusCode set_level(Level) override {
        return status::StatusCode::OK;
    }

    //! Non-operational state flipping.
    status::StatusCode flip() override {
        return status::StatusCode::OK;
    }

    //! Non-operational GPIO enabling.
    status::StatusCode turn_on() override {
        return status::StatusCode::OK;
    }

    //! Non-operational GPIO disabling.
    status::StatusCode turn_off() override {
        return status::StatusCode::OK;
    }

    //! Non-operational direction setting.
    status::StatusCode set_direction(Direction) override {
        return status::StatusCode::OK;
    }

private:
    const GpioNum gpio_num_ { static_cast<GpioNum>(0) };
};

} // namespace gpio
} // namespace io
} // namespace ocs
