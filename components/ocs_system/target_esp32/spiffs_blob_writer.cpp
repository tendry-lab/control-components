/*
 * SPDX-FileCopyrightText: 2025-2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_spiffs.h"

#include "ocs_core/freertos.h"
#include "ocs_core/log.h"
#include "ocs_system/target_esp32/spiffs_blob_writer.h"

namespace ocs {
namespace system {

namespace {

const char* log_tag = "spiffs_blob_writer";

} // namespace

SpiffsBlobWriter::SpiffsBlobWriter(IBlobWriter& writer, const char* partition_label)
    : partition_label_(partition_label)
    , writer_(writer) {
}

status::StatusCode SpiffsBlobWriter::begin(uint32_t total_size, uint32_t crc32) {
    const auto err = esp_vfs_spiffs_unregister(partition_label_.c_str());
    if (err != ESP_OK) {
        ocs_loge(log_tag, "esp_vfs_spiffs_unregister(): %s", esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return writer_.begin(total_size, crc32);
}

status::StatusCode SpiffsBlobWriter::write(const uint8_t* buf, size_t len) {
    return writer_.write(buf, len);
}

status::StatusCode SpiffsBlobWriter::commit() {
    return writer_.commit();
}

status::StatusCode SpiffsBlobWriter::end() {
    return writer_.end();
}

} // namespace system
} // namespace ocs
