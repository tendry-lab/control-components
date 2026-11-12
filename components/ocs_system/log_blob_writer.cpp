/*
 * SPDX-FileCopyrightText: 2025-2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_system/log_blob_writer.h"
#include "ocs_algo/math_ops.h"
#include "ocs_core/freertos.h"
#include "ocs_core/log.h"

namespace ocs {
namespace system {

LogBlobWriter::LogBlobWriter(IBlobWriter& writer, const char* log_tag)
    : log_tag_(log_tag)
    , writer_(writer) {
}

status::StatusCode LogBlobWriter::begin(uint32_t total_size, uint32_t crc32) {
    ocs_logi(log_tag_.c_str(), "begin writing blob: total_size=%lu crc32=%lu", total_size,
             crc32);

    total_size_ = total_size;
    written_size_ = 0;

    return writer_.begin(total_size, crc32);
}

status::StatusCode LogBlobWriter::write(const uint8_t* buf, size_t len) {
    const auto code = writer_.write(buf, len);
    if (code != ocs::status::StatusCode::OK) {
        return code;
    }

    configASSERT(total_size_);

    written_size_ += len;

    float progress = 100 * (static_cast<float>(written_size_) / total_size_);
    progress = algo::MathOps::round_floor(progress, 1);

    ocs_logi(log_tag_.c_str(), "writing blob data: progress=%.1f", progress);

    return ocs::status::StatusCode::OK;
}

status::StatusCode LogBlobWriter::commit() {
    ocs_logi(log_tag_.c_str(), "committing blob");

    return writer_.commit();
}

status::StatusCode LogBlobWriter::end() {
    ocs_logi(log_tag_.c_str(), "ending blob");

    return writer_.end();
}

} // namespace system
} // namespace ocs
