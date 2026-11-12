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

//! Log the blob writing progress.
class LogBlobWriter : public IBlobWriter, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p writer to write the blob.
    //!  - @p log_tag to use for logging.
    LogBlobWriter(IBlobWriter& writer, const char* log_tag);

    //! Begin writing the blob.
    status::StatusCode begin(uint32_t total_size, uint32_t crc32) override;

    //! Write blob data.
    status::StatusCode write(const uint8_t* buf, size_t len) override;

    //! Commit the blob.
    status::StatusCode commit() override;

    //! End writing the blob.
    status::StatusCode end() override;

private:
    const std::string log_tag_;

    uint32_t total_size_ { 0 };
    uint32_t written_size_ { 0 };

    IBlobWriter& writer_;
};

} // namespace system
} // namespace ocs
