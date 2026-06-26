/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_system/log_updater.h"
#include "ocs_algo/math_ops.h"
#include "ocs_core/freertos.h"
#include "ocs_core/log.h"

namespace ocs {
namespace system {

LogUpdater::LogUpdater(IUpdater& updater, const char* log_tag)
    : log_tag_(log_tag)
    , updater_(updater) {
}

status::StatusCode LogUpdater::begin(uint32_t total_size, uint32_t crc32) {
    ocs_logi(log_tag_.c_str(), "beginning firmware update: total_size=%lu crc32=%lu",
             total_size, crc32);

    total_size_ = total_size;
    written_size_ = 0;

    return updater_.begin(total_size, crc32);
}

status::StatusCode LogUpdater::write(const uint8_t* buf, size_t len) {
    const auto code = updater_.write(buf, len);
    if (code != ocs::status::StatusCode::OK) {
        return code;
    }

    configASSERT(total_size_);

    written_size_ += len;

    float progress = 100 * (static_cast<float>(written_size_) / total_size_);
    progress = algo::MathOps::round_floor(progress, 1);

    ocs_logi(log_tag_.c_str(), "writing firmware data: progress=%.1f", progress);

    return ocs::status::StatusCode::OK;
}

status::StatusCode LogUpdater::commit() {
    ocs_logi(log_tag_.c_str(), "committing firmware update");

    return updater_.commit();
}

status::StatusCode LogUpdater::end() {
    ocs_logi(log_tag_.c_str(), "ending firmware update");

    return updater_.end();
}

} // namespace system
} // namespace ocs
