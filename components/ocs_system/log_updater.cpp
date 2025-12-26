/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/log.h"
#include "ocs_system/log_updater.h"

namespace ocs {
namespace system {

LogUpdater::LogUpdater(IUpdater& updater, const char* log_tag)
    : log_tag_(log_tag)
    , updater_(updater) {
}

status::StatusCode LogUpdater::begin(size_t total_size, uint32_t crc32) {
    ocs_logi(log_tag_.c_str(), "beginning firmware update: total_size=%zu crc32=%lu",
             total_size, crc32);

    return updater_.begin(total_size, crc32);
}

status::StatusCode LogUpdater::write(const uint8_t* buf, size_t len) {
    ocs_logd(log_tag_.c_str(), "writing firmware data: len=%zu", len);

    return updater_.write(buf, len);
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
