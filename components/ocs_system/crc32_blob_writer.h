/*
 * SPDX-FileCopyrightText: 2025-2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_system/iblob_writer.h"
#include "ocs_system/icrc32_calculator.h"

namespace ocs {
namespace system {

//! Verify CRC32 of the blob before committing it.
class Crc32BlobWriter : public IBlobWriter, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p crc32_calculator to calculate CRC32 checksum to check data integrity.
    //!  - @p writer to write the blob.
    Crc32BlobWriter(ICrc32Calculator& crc32_calculator, IBlobWriter& writer);

    //! Begin writing the blob.
    status::StatusCode begin(uint32_t total_size, uint32_t crc32) override;

    //! Write blob data.
    status::StatusCode write(const uint8_t* buf, size_t len) override;

    //! Verify CRC32 of the written data and commit the blob.
    status::StatusCode commit() override;

    //! End writing the blob.
    status::StatusCode end() override;

private:
    ICrc32Calculator& crc32_calculator_;
    IBlobWriter& writer_;

    uint32_t crc32_src_ { 0 };
    uint32_t crc32_clc_ { 0 };
};

} // namespace system
} // namespace ocs
