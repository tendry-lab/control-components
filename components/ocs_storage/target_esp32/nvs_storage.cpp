/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <algorithm>
#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/log.h"
#include "ocs_storage/target_esp32/nvs_storage.h"

namespace ocs {
namespace storage {

namespace {

const char* log_tag = "nvs_storage";

} // namespace

NvsStorage::NvsStorage(const char* ns) {
    configASSERT(strlen(ns) <= max_namespace_len);

    memset(ns_, 0, sizeof(ns_));
    strncpy(ns_, ns, std::min(max_namespace_len, strlen(ns)));
}

status::StatusCode NvsStorage::probe(const char* key, size_t& size) {
    configASSERT(key);

    auto [handle, code] = open_(NVS_READONLY);
    if (code == status::StatusCode::OK) {
        code = read_(handle, key, nullptr, size);
        nvs_close(handle);
    }

    return code;
}

status::StatusCode NvsStorage::read(const char* key, void* value, size_t size) {
    configASSERT(key);
    configASSERT(value);
    configASSERT(size);

    auto [handle, code] = open_(NVS_READONLY);
    if (code == status::StatusCode::OK) {
        code = read_(handle, key, value, size);
        nvs_close(handle);
    }

    return code;
}

status::StatusCode NvsStorage::write(const char* key, const void* value, size_t size) {
    configASSERT(key);
    configASSERT(value);
    configASSERT(size);

    auto [handle, code] = open_(NVS_READWRITE);
    if (code == status::StatusCode::OK) {
        code = write_(handle, key, value, size);
        nvs_close(handle);
    }

    return code;
}

status::StatusCode NvsStorage::erase(const char* key) {
    configASSERT(key);

    auto [handle, code] = open_(NVS_READWRITE);
    if (code == status::StatusCode::OK) {
        code = erase_(handle, key);
        nvs_close(handle);
    }

    return code;
}

status::StatusCode NvsStorage::erase_all() {
    auto [handle, code] = open_(NVS_READWRITE);
    if (code == status::StatusCode::OK) {
        code = erase_all_(handle);
        nvs_close(handle);
    }

    return code;
}

std::pair<nvs_handle_t, status::StatusCode> NvsStorage::open_(nvs_open_mode_t mode) {
    nvs_handle_t handle = 0;

    const auto err = nvs_open(ns_, mode, &handle);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        return { 0, status::StatusCode::NoData };
    }

    if (err != ESP_OK) {
        ocs_loge(log_tag, "nvs_open(): %s", esp_err_to_name(err));
        return { 0, status::StatusCode::Error };
    }

    return { handle, status::StatusCode::OK };
}

status::StatusCode
NvsStorage::read_(nvs_handle_t handle, const char* key, void* value, size_t& size) {
    const auto err = nvs_get_blob(handle, key, value, &size);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        return status::StatusCode::NoData;
    }

    if (err != ESP_OK) {
        ocs_loge(log_tag, "failed to read: nvs_get_blob(): key=%s err=%s", key,
                 esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

status::StatusCode
NvsStorage::write_(nvs_handle_t handle, const char* key, const void* value, size_t size) {
    auto err = nvs_set_blob(handle, key, value, size);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "failed to write: nvs_set_blob(): key=%s err=%s", key,
                 esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    err = nvs_commit(handle);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "failed to write: nvs_commit(): key=%s err=%s", key,
                 esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

status::StatusCode NvsStorage::erase_(nvs_handle_t handle, const char* key) {
    auto err = nvs_erase_key(handle, key);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        return status::StatusCode::NoData;
    }

    if (err != ESP_OK) {
        ocs_loge(log_tag, "failed to erase: nvs_erase_key(): key=%s err=%s", key,
                 esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    err = nvs_commit(handle);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "failed to erase: nvs_commit(): key=%s err=%s", key,
                 esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

status::StatusCode NvsStorage::erase_all_(nvs_handle_t handle) {
    auto err = nvs_erase_all(handle);
    if (err == ESP_ERR_NVS_NOT_FOUND) {
        return status::StatusCode::NoData;
    }

    if (err != ESP_OK) {
        ocs_loge(log_tag, "failed to erase: nvs_erase_all(): err=%s",
                 esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    err = nvs_commit(handle);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "failed to erase all: nvs_commit(): err=%s",
                 esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

} // namespace storage
} // namespace ocs
