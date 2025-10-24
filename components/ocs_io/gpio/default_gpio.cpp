/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_io/gpio/default_gpio.h"

namespace ocs {
namespace io {
namespace gpio {

DefaultGpio::DefaultGpio(const char* id, Gpio gpio)
    : BasicGpio(id, gpio, true) {
}

} // namespace gpio
} // namespace io
} // namespace ocs
