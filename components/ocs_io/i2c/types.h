/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstdint>

namespace ocs {
namespace io {
namespace i2c {

enum class TransferSpeed : uint8_t {
    //! 100 kbit/s.
    Default,

    //! 400 kbit/s.
    Fast,
};

enum class AddressLength : uint8_t {
    //! 7-bit I2C address.
    Bit_7,

    //! 10-bit I2C address.
    Bit_10,
};

//! I2C address.
using Address = uint16_t;

} // namespace i2c
} // namespace io
} // namespace ocs
