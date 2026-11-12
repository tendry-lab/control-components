/*
 * SPDX-FileCopyrightText: 2025-2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_system/crc32_blob_writer.h"
#include "ocs_core/freertos.h"
#include "ocs_core/log.h"

namespace ocs {
namespace system {

namespace {

const char* log_tag = "crc32_blob_writer";

} // namespace

Crc32BlobWriter::Crc32BlobWriter(ICrc32Calculator& crc32_calculator, IBlobWriter& writer)
    : crc32_calculator_(crc32_calculator)
    , writer_(writer) {
}

status::StatusCode Crc32BlobWriter::begin(uint32_t total_size, uint32_t crc32) {
    const auto code = writer_.begin(total_size, crc32);
    if (code != status::StatusCode::OK) {
        return code;
    }

    crc32_src_ = crc32;
    crc32_clc_ = 0;

    return status::StatusCode::OK;
}

status::StatusCode Crc32BlobWriter::write(const uint8_t* buf, size_t len) {
    const auto code = writer_.write(buf, len);
    if (code != status::StatusCode::OK) {
        return code;
    }

    crc32_clc_ = crc32_calculator_.calculate(crc32_clc_, buf, len);

    return status::StatusCode::OK;
}

status::StatusCode Crc32BlobWriter::commit() {
    configASSERT(crc32_src_);
    configASSERT(crc32_clc_);

    if (crc32_src_ != crc32_clc_) {
        ocs_loge(log_tag, "integrity check failed: crc32 mismatch: want=%lu got=%lu",
                 crc32_src_, crc32_clc_);

        return status::StatusCode::Error;
    }

    const auto code = writer_.commit();
    if (code != status::StatusCode::OK) {
        return code;
    }

    return status::StatusCode::OK;
}

status::StatusCode Crc32BlobWriter::end() {
    return writer_.end();
}

} // namespace system
} // namespace ocs
