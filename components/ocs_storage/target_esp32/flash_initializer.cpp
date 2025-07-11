/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "esp_err.h"
#include "nvs_flash.h"

#include "ocs_storage/target_esp32/flash_initializer.h"

namespace ocs {
namespace storage {

FlashInitializer::FlashInitializer() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

FlashInitializer::~FlashInitializer() {
    ESP_ERROR_CHECK(nvs_flash_deinit());
}

} // namespace storage
} // namespace ocs
