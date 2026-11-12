/*
 * SPDX-FileCopyrightText: 2025-2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include "ocs_status/code.h"

namespace ocs {
namespace system {

//! Write a blob of bytes: firmware image, file system image, arbitrary payload.
//!
//! @notes
//!  - A blob is written in the following order: begin(), write() one or more times,
//!    commit(), end().
//!
//!  - The written data is accepted only by a successful commit() and takes effect only
//!    after the end() is called.
//!
//!  - end() without a successful commit() aborts the blob: the written data is discarded.
class IBlobWriter {
public:
    //! Destroy.
    virtual ~IBlobWriter() = default;

    //! Begin writing the blob.
    //!
    //! @params
    //!  - @p total_size - total blob size, in bytes.
    //!  - @p crc32 - CRC32 of the whole blob, to verify data integrity.
    //!
    //! @remarks
    //!  Fails if the blob can't be accepted, for example if @p total_size is too big.
    //!  Calling begin() while a blob is being written restarts the process, the data
    //!  written so far is discarded.
    virtual status::StatusCode begin(uint32_t total_size, uint32_t crc32) = 0;

    //! Write the next part of the blob.
    //!
    //! @params
    //!  - @p buf - blob data, should be at least @p len bytes long.
    //!  - @p len - data size, in bytes.
    virtual status::StatusCode write(const uint8_t* buf, size_t len) = 0;

    //! Accept the written data.
    //!
    //! @remarks
    //!  Fails if the written data is incomplete or invalid, in that case end() aborts
    //!  the blob.
    virtual status::StatusCode commit() = 0;

    //! End writing the blob, release the resources acquired in begin().
    //!
    //! @remarks
    //!  Applies the blob if commit() has succeeded, aborts it otherwise.
    virtual status::StatusCode end() = 0;
};

} // namespace system
} // namespace ocs
