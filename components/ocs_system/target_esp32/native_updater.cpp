/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/log.h"
#include "ocs_system/target_esp32/native_updater.h"

namespace ocs {
namespace system {

namespace {

const char* log_tag = "native_updater";

} // namespace

NativeUpdater::NativeUpdater(ICrc32Calculator& crc32_calculator)
    : crc32_calculator_(crc32_calculator) {
}

status::StatusCode NativeUpdater::begin(size_t total_size, uint32_t crc32) {
    configASSERT(!handle_);
    configASSERT(!crc32_src_);
    configASSERT(!crc32_clc_);

    crc32_src_ = crc32;

    const auto partition = esp_ota_get_next_update_partition(nullptr);
    if (!partition) {
        ocs_loge(log_tag, "OTA partion not found");

        return status::StatusCode::InvalidState;
    }

    const auto err = esp_ota_begin(partition, total_size, &handle_);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "esp_ota_begin(): %s", esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    ocs_logi(log_tag, "begin update process: total_size=%zu crc32=%lu", total_size,
             crc32);

    return status::StatusCode::OK;
}

status::StatusCode NativeUpdater::write(const uint8_t* buf, size_t len) {
    configASSERT(handle_);

    const auto err = esp_ota_write(handle_, buf, len);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "esp_ota_write(): %s", esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    crc32_clc_ = crc32_calculator_.calculate(crc32_clc_, buf, len);

    return status::StatusCode::OK;
}

status::StatusCode NativeUpdater::commit() {
    configASSERT(!committed_);
    configASSERT(crc32_src_);
    configASSERT(crc32_clc_);

    if (crc32_src_ != crc32_clc_) {
        ocs_loge(log_tag, "integrity check failed: crc32 mismatch: want=%lu got=%lu",
                 crc32_src_, crc32_clc_);

        return status::StatusCode::Error;
    }

    const auto partition = esp_ota_get_next_update_partition(nullptr);
    configASSERT(partition);

    const auto err = esp_ota_set_boot_partition(partition);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "esp_ota_set_boot_partition(): %s", esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    committed_ = true;

    return status::StatusCode::OK;
}

status::StatusCode NativeUpdater::end() {
    configASSERT(handle_);

    if (committed_) {
        const auto err = esp_ota_end(handle_);
        if (err != ESP_OK) {
            ocs_loge(log_tag, "esp_ota_end(): %s", esp_err_to_name(err));

            return status::StatusCode::Error;
        }
    } else {
        const auto err = esp_ota_abort(handle_);
        if (err != ESP_OK) {
            ocs_loge(log_tag, "esp_ota_abort(): %s", esp_err_to_name(err));

            return status::StatusCode::Error;
        }
    }

    ocs_logi(log_tag, "end update process: committed=%d", committed_);

    handle_ = 0;
    crc32_src_ = 0;
    crc32_clc_ = 0;
    committed_ = false;

    return status::StatusCode::OK;
}

} // namespace system
} // namespace ocs
