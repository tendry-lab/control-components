/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstring>

#include "esp_spiffs.h"

#include "ocs_core/log.h"
#include "ocs_storage/target_esp32/spiffs_initializer.h"

namespace ocs {
namespace storage {

namespace {

const char* log_tag = "spiffs_initializer";

} // namespace

SpiffsInitializer::SpiffsInitializer(const char* mount_point) {
    esp_vfs_spiffs_conf_t fs_configuration;
    memset(&fs_configuration, 0, sizeof(fs_configuration));

    fs_configuration.base_path = mount_point;
    fs_configuration.max_files = 5;

    auto ret = esp_vfs_spiffs_register(&fs_configuration);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ocs_loge(log_tag, "failed to mount or format filesystem: mount_point=%s",
                     mount_point);
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ocs_loge(log_tag, "failed to find SPIFFS partition: mount_point=%s",
                     mount_point);
        } else {
            ocs_loge(log_tag, "failed to initialize SPIFFS: mount_point=%s err=%s",
                     mount_point, esp_err_to_name(ret));
        }

        return;
    }

    size_t total = 0;
    size_t used = 0;

    ret = esp_spiffs_info(nullptr, &total, &used);
    if (ret != ESP_OK) {
        ocs_loge(log_tag, "esp_spiffs_info(): %s", esp_err_to_name(ret));

        return;
    }

    ocs_logi(log_tag, "partition size: total=%d used=%d", total, used);

    valid_ = true;
}

bool SpiffsInitializer::valid() const {
    return valid_;
}

} // namespace storage
} // namespace ocs
