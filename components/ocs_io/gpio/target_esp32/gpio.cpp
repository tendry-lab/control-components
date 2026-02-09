/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/log.h"
#include "ocs_io/gpio/target_esp32/gpio.h"

namespace ocs {
namespace io {
namespace gpio {

namespace {

const char* log_tag = "gpio";

} // namespace

Gpio::Gpio(gpio_num_t gpio_num, Level enable_level)
    : gpio_num_(gpio_num)
    , enable_level_(enable_level) {
}

GpioNum Gpio::get_num() const {
    return gpio_num_;
}

status::StatusCode Gpio::get_level(Level& level) {
    const int value = gpio_get_level(gpio_num_);
    if (value < 0) {
        return status::StatusCode::Error;
    }

    level = static_cast<Level>(value);

    return status::StatusCode::OK;
}

status::StatusCode Gpio::flip() {
    Level level = 0;

    const auto code = get_level(level);
    if (code != ocs::status::StatusCode::OK) {
        return code;
    }

    return level == enable_level_ ? turn_off() : turn_on();
}

status::StatusCode Gpio::turn_on() {
    const auto err = gpio_set_level(gpio_num_, enable_level_);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "gpio_set_level(%d,%u): %s", gpio_num_, enable_level_,
                 esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

status::StatusCode Gpio::turn_off() {
    const auto err = gpio_set_level(gpio_num_, !enable_level_);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "gpio_set_level(%d,%u): %s", gpio_num_, enable_level_,
                 esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

status::StatusCode Gpio::set_direction(Direction direction) {
    gpio_mode_t mode = GPIO_MODE_DISABLE;

    switch (direction) {
    case Direction::Output:
        mode = GPIO_MODE_OUTPUT;
        break;

    case Direction::Input:
        mode = GPIO_MODE_INPUT;
        break;

    default:
        break;
    }

    configASSERT(mode != GPIO_MODE_DISABLE);

    const auto err = gpio_set_direction(gpio_num_, mode);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "gpio_set_direction(): %s", esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

} // namespace gpio
} // namespace io
} // namespace ocs
