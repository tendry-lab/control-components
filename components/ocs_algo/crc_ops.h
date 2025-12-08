/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstddef>
#include <cstdint>

namespace ocs {
namespace algo {

struct CrcOps {
    //! https://en.wikipedia.org/wiki/Bit_numbering.
    enum class BitOrder : uint8_t {
        //! Least significan bit first.
        LSB,
        //! Most significan bit first.
        MSB,
    };

    //! Calculate CRC-8 checksum.
    //!
    //! @params
    //!
    //! @references
    //!  https://www.boost.org/doc/libs/1_84_0/doc/html/crc/crc_optimal.html
    static uint8_t crc8(const uint8_t* buf,
                        size_t size,
                        uint8_t initial,
                        uint8_t polynomial,
                        BitOrder order);
};

} // namespace algo
} // namespace ocs
