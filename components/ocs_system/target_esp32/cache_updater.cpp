/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <algorithm>

#include "freertos/FreeRTOSConfig.h"

#include "esp_ota_ops.h"

#include "ocs_core/log.h"
#include "ocs_system/target_esp32/cache_updater.h"

namespace ocs {
namespace system {

namespace {

const char* log_tag = "cache_updater";

} // namespace

CacheUpdater::CacheUpdater(IUpdater& updater, size_t chunk_size)
    : chunk_size_(chunk_size)
    , updater_(updater) {
}

status::StatusCode CacheUpdater::begin(size_t total_size, uint32_t crc32) {
    offset_ = 0;
    total_size_ = total_size;
    crc32_ = crc32;

    partition_ = esp_ota_get_next_update_partition(nullptr);
    if (!partition_) {
        ocs_loge(log_tag, "OTA partion not found");

        return status::StatusCode::InvalidState;
    }

    const auto err = esp_partition_erase_range(partition_, 0, partition_->size);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "esp_partition_erase_range(): %s", esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

status::StatusCode CacheUpdater::write(const uint8_t* buf, size_t len) {
    configASSERT(partition_);

    const auto err = esp_partition_write(partition_, offset_, buf, len);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "esp_partition_write(): %s", esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    offset_ += len;

    return status::StatusCode::OK;
}

status::StatusCode CacheUpdater::commit() {
    configASSERT(offset_);
    configASSERT(partition_);
    configASSERT(total_size_);
    configASSERT(crc32_);

    auto code = updater_.begin(total_size_, crc32_);
    if (code != status::StatusCode::OK) {
        return code;
    }

    code = write_();
    if (code != status::StatusCode::OK) {
        return code;
    }

    code = updater_.commit();
    if (code != status::StatusCode::OK) {
        return code;
    }

    code = updater_.end();
    if (code != status::StatusCode::OK) {
        return code;
    }

    return status::StatusCode::OK;
}

status::StatusCode CacheUpdater::end() {
    partition_ = nullptr;

    return status::StatusCode::OK;
}

status::StatusCode CacheUpdater::write_() {
    esp_partition_mmap_handle_t handle = 0;
    const void* ptr = nullptr;

    const auto err = esp_partition_mmap(partition_, 0, offset_, ESP_PARTITION_MMAP_DATA,
                                        &ptr, &handle);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "esp_partition_mmap(): %s", esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    status::StatusCode code = status::StatusCode::OK;

    size_t remaining = offset_;
    size_t pos = 0;

    while (remaining) {
        const size_t len = std::min(remaining, chunk_size_);

        code = updater_.write(reinterpret_cast<const uint8_t*>(ptr) + pos, len);
        if (code != status::StatusCode::OK) {
            break;
        }

        remaining -= len;
        pos += len;
    }

    esp_partition_munmap(handle);

    return code;
}

} // namespace system
} // namespace ocs
