/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include "ocs_status/code.h"

namespace ocs {
namespace system {

//! Firmware updater.
class IUpdater {
public:
    //! Destroy.
    virtual ~IUpdater() = default;

    //! Begin firmware update.
    //!
    //! @params
    //!  - @p total_size - total firmware size, in bytes.
    //!  - @p crc32 - CRC32 to verify data integrity.
    virtual status::StatusCode begin(size_t total_size, uint32_t crc32) = 0;

    //! Write firmware data.
    //!
    //! @params
    //!  - @p buf - buffer with firmware, should be at least @p len bytes long.
    //!  - @p len - buffer size, in bytes.
    virtual status::StatusCode write(const uint8_t* buf, size_t len) = 0;

    //! Commit written firmware data.
    virtual status::StatusCode commit() = 0;

    //! End firmware update.
    virtual status::StatusCode end() = 0;
};

} // namespace system
} // namespace ocs
