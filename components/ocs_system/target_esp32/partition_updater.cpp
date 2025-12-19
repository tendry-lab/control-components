/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/log.h"
#include "ocs_system/target_esp32/partition_updater.h"

namespace ocs {
namespace system {

namespace {

const char* log_tag = "partition_updater";

} // namespace

PartitionUpdater::PartitionUpdater(esp_partition_type_t type,
                                   esp_partition_subtype_t subtype,
                                   const char* partname) {
    partition_ = esp_partition_find_first(type, subtype, partname);
    configASSERT(partition_);
}

status::StatusCode PartitionUpdater::begin(size_t total_size, uint32_t crc32) {
    offset_ = 0;

    const auto err = esp_partition_erase_range(partition_, 0, partition_->size);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "esp_partition_erase_range(): %s", esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

status::StatusCode PartitionUpdater::write(const uint8_t* buf, size_t len) {
    configASSERT(partition_);

    const auto err = esp_partition_write(partition_, offset_, buf, len);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "esp_partition_write(): %s", esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    offset_ += len;

    return status::StatusCode::OK;
}

status::StatusCode PartitionUpdater::commit() {
    configASSERT(offset_);

    return status::StatusCode::OK;
}

status::StatusCode PartitionUpdater::end() {
    return status::StatusCode::OK;
}

} // namespace system
} // namespace ocs
