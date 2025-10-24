/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_io/gpio/basic_gpio.h"

namespace ocs {
namespace io {
namespace gpio {

//! High logic level is used to enable GPIO.
class DefaultGpio : public BasicGpio, private core::NonCopyable<> {
public:
    //! Initialize.
    DefaultGpio(const char* id, Gpio gpio);
};

} // namespace gpio
} // namespace io
} // namespace ocs
