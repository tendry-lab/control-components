/*
 * SPDX-FileCopyrightText: 2025-2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <string>

#include "ocs_core/noncopyable.h"
#include "ocs_system/iblob_writer.h"

namespace ocs {
namespace system {

//! Unmount the SPIFFS partition before writing the blob to it.
class SpiffsBlobWriter : public IBlobWriter, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p writer - to write the blob to the SPIFFS partition.
    //!  - @p partition_label - SPIFFS partition label.
    SpiffsBlobWriter(IBlobWriter& writer, const char* partition_label);

    //! Unmount the SPIFFS partition and begin writing the blob.
    status::StatusCode begin(uint32_t total_size, uint32_t crc32) override;

    //! Write blob data.
    status::StatusCode write(const uint8_t* buf, size_t len) override;

    //! Commit the blob.
    status::StatusCode commit() override;

    //! End writing the blob.
    status::StatusCode end() override;

private:
    const std::string partition_label_;

    IBlobWriter& writer_;
};

} // namespace system
} // namespace ocs
