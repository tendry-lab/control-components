/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOSConfig.h"

#include "esp_spiffs.h"

#include "ocs_core/log.h"
#include "ocs_system/target_esp32/spiffs_updater.h"

namespace ocs {
namespace system {

namespace {

const char* log_tag = "spiffs_updater";

} // namespace

SpiffsUpdater::SpiffsUpdater(IUpdater& updater, const char* partition_label)
    : partition_label_(partition_label)
    , updater_(updater) {
}

status::StatusCode SpiffsUpdater::begin(size_t total_size, uint32_t crc32) {
    const auto err = esp_vfs_spiffs_unregister(partition_label_.c_str());
    if (err != ESP_OK) {
        ocs_loge(log_tag, "esp_vfs_spiffs_unregister(): %s", esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return updater_.begin(total_size, crc32);
}

status::StatusCode SpiffsUpdater::write(const uint8_t* buf, size_t len) {
    return updater_.write(buf, len);
}

status::StatusCode SpiffsUpdater::commit() {
    return updater_.commit();
}

status::StatusCode SpiffsUpdater::end() {
    return updater_.end();
}

} // namespace system
} // namespace ocs
