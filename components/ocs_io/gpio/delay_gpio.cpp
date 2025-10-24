/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_io/gpio/delay_gpio.h"
#include "ocs_status/macros.h"

namespace ocs {
namespace io {
namespace gpio {

DelayGpio::DelayGpio(IGpio& gpio, DelayGpio::Params params)
    : params_(params)
    , gpio_(gpio) {
}

int DelayGpio::get() {
    return gpio_.get();
}

status::StatusCode DelayGpio::flip() {
    OCS_STATUS_RETURN_ON_ERROR(gpio_.flip());

    if (params_.flip_delay_interval) {
        vTaskDelay(params_.flip_delay_interval);
    }

    return status::StatusCode::OK;
}

status::StatusCode DelayGpio::turn_on() {
    OCS_STATUS_RETURN_ON_ERROR(gpio_.turn_on());

    if (params_.turn_on_delay_interval) {
        vTaskDelay(params_.turn_on_delay_interval);
    }

    return status::StatusCode::OK;
}

status::StatusCode DelayGpio::turn_off() {
    OCS_STATUS_RETURN_ON_ERROR(gpio_.turn_off());

    if (params_.turn_off_delay_interval) {
        vTaskDelay(params_.turn_off_delay_interval);
    }

    return status::StatusCode::OK;
}

status::StatusCode DelayGpio::set_direction(IGpio::Direction direction) {
    return gpio_.set_direction(direction);
}

} // namespace gpio
} // namespace io
} // namespace ocs
