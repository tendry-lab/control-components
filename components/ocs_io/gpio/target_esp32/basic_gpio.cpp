/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/log.h"
#include "ocs_io/gpio/basic_gpio.h"

namespace ocs {
namespace io {
namespace gpio {

namespace {

const char* log_tag = "basic_gpio";

} // namespace

BasicGpio::BasicGpio(const char* id, Gpio gpio, bool enable_value)
    : id_(id)
    , gpio_(gpio)
    , enable_value_(enable_value) {
}

int BasicGpio::get() {
    return gpio_get_level(gpio_);
}

status::StatusCode BasicGpio::flip() {
    return get() == enable_value_ ? turn_off() : turn_on();
}

status::StatusCode BasicGpio::turn_on() {
    const auto err = gpio_set_level(gpio_, enable_value_);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "turn on failed: id=%s err=%s", id_.c_str(),
                 esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

status::StatusCode BasicGpio::turn_off() {
    const auto err = gpio_set_level(gpio_, !enable_value_);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "turn off failed: id=%s err=%s", id_.c_str(),
                 esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

status::StatusCode BasicGpio::set_direction(IGpio::Direction direction) {
    gpio_mode_t mode = GPIO_MODE_DISABLE;

    switch (direction) {
    case IGpio::Direction::Output:
        mode = GPIO_MODE_OUTPUT;
        break;

    case IGpio::Direction::Input:
        mode = GPIO_MODE_INPUT;
        break;

    default:
        break;
    }

    configASSERT(mode != GPIO_MODE_DISABLE);

    const auto err = gpio_set_direction(gpio_, mode);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "gpio_set_direction(): %s", esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

} // namespace gpio
} // namespace io
} // namespace ocs
