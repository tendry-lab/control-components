/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstdint>

#ifdef CONFIG_OCS_FIRMWARE_PLATFORM_STM32

#else // !CONFIG_OCS_FIRMWARE_PLATFORM_STM32

#include "driver/gpio.h"

#endif // CONFIG_OCS_FIRMWARE_PLATFORM_STM32

namespace ocs {
namespace io {
namespace gpio {

#ifdef CONFIG_OCS_FIRMWARE_PLATFORM_STM32

//! GPIO number.
using GpioNum = uint16_t;

#else // !CONFIG_OCS_FIRMWARE_PLATFORM_STM32

//! GPIO number.
using GpioNum = gpio_num_t;

#endif // CONFIG_OCS_FIRMWARE_PLATFORM_STM32

//! GPIO directions.
enum class Direction : uint8_t {
    Output,
    Input,
};

//! GPIO level.
using Level = uint8_t;

} // namespace gpio
} // namespace io
} // namespace ocs
