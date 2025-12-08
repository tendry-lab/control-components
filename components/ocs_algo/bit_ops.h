/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstdint>

namespace ocs {
namespace algo {

struct BitOps {
    //! Create a mask with the n-th bit set.
    static uint32_t mask(uint8_t pos);

    //! Create a mask with the n-th bit unset.
    static uint32_t umask(uint8_t pos);

    //! Return the n-th bit of the value.
    static uint8_t nth(uint32_t value, uint8_t pos);

    //! Return 16-bit value from two 8-bit values.
    static uint16_t pack_u8(uint8_t hi, uint8_t lo);

    //! Return 32-bit value from two 16-bit values.
    static uint32_t pack_u16(uint16_t hi, uint16_t lo);

    //! Unpack 16-bit value into corresponding 8-bit values.
    static void unpack_u8(uint16_t value, uint8_t& hi, uint8_t& lo);
};

} // namespace algo
} // namespace ocs
