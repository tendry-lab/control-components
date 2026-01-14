/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstdint>

#include "driver/gpio.h"

namespace ocs {
namespace io {
namespace gpio {

//! GPIO number.
using GpioNum = gpio_num_t;

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
