/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstddef>
#include <cstdint>

namespace ocs {
namespace system {

class ICrc32Calculator {
public:
    //! Destroy.
    virtual ~ICrc32Calculator() = default;

    //! Calculate CRC32.
    //!
    //! @params
    //!  - @p crc - seed value, or the previous crc32 value if computing incrementally.
    //!  - @p buf - buffer over which CRC is run, should be at least @p len bytes long.
    //!  - @p len - buffer size, in bytes.
    virtual uint32_t calculate(uint32_t crc, const uint8_t* buf, size_t len) = 0;
};

} // namespace system
} // namespace ocs
