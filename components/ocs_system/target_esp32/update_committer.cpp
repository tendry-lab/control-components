/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_ota_ops.h"

#include "ocs_core/log.h"
#include "ocs_system/target_esp32/update_committer.h"

namespace ocs {
namespace system {

namespace {

const char* log_tag = "update_committer";

} // namespace

bool UpdateCommitter::require() {
    esp_ota_img_states_t ota_state;
    const auto err =
        esp_ota_get_state_partition(esp_ota_get_running_partition(), &ota_state);
    if (err != ESP_OK) {
        ocs_logw(log_tag, "esp_ota_get_state_partition(): %s", esp_err_to_name(err));

        return false;
    }

    if (ota_state != ESP_OTA_IMG_PENDING_VERIFY) {
        return false;
    }

    return true;
}

status::StatusCode UpdateCommitter::commit() {
    const auto err = esp_ota_mark_app_valid_cancel_rollback();
    if (err != ESP_OK) {
        ocs_logw(log_tag, "esp_ota_mark_app_valid_cancel_rollback(): %s",
                 esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

} // namespace system
} // namespace ocs
