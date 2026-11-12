/*
 * SPDX-FileCopyrightText: 2025-2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_partition.h"

#include "ocs_core/noncopyable.h"
#include "ocs_system/iblob_writer.h"

namespace ocs {
namespace system {

//! Cache the blob in the next OTA partition.
//!
//! @remarks
//!  Don't use this component while the firmware update is in progress,
//!  because it also writes the new FW image data into the next OTA partition.
class CacheBlobWriter : public IBlobWriter, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p writer to write the cached blob to.
    //!  - @p chunk_size to use when writing data from the cached partition.
    CacheBlobWriter(IBlobWriter& writer, size_t chunk_size);

    //! Erase the cache partition.
    status::StatusCode begin(uint32_t total_size, uint32_t crc32) override;

    //! Write blob data to the cache partition.
    status::StatusCode write(const uint8_t* buf, size_t len) override;

    //! Write the cached blob to the underlying writer.
    status::StatusCode commit() override;

    //! End writing the blob.
    status::StatusCode end() override;

private:
    status::StatusCode write_();

    const size_t chunk_size_ { 0 };

    IBlobWriter& writer_;

    const esp_partition_t* partition_ { nullptr };
    uint32_t total_size_ { 0 };
    size_t crc32_ { 0 };
    size_t offset_ { 0 };
};

} // namespace system
} // namespace ocs
