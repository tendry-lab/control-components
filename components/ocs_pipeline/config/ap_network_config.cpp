/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "ocs_algo/storage_ops.h"
#include "ocs_core/log.h"
#include "ocs_pipeline/config/ap_network_config.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace pipeline {
namespace config {

namespace {

const char* log_tag = "ap_network_config";

} // namespace

ApNetworkConfig::ApNetworkConfig(storage::IStorage& storage,
                                 const system::DeviceInfo& device_info) {
    memset(ssid_, 0, sizeof(ssid_));
    memset(password_, 0, sizeof(password_));

    std::string builtin_ssid = device_info.get_fw_name();
    builtin_ssid += "-";
    builtin_ssid += device_info.get_device_id();

    strncpy(ssid_, builtin_ssid.c_str(), sizeof(ssid_));

    const auto code = algo::StorageOps::prob_read(storage, password_key_, password_,
                                                  max_password_size_);
    if (code != status::StatusCode::OK) {
        if (code != status::StatusCode::NoData) {
            ocs_loge(log_tag, "failed to read WiFi AP password from storage: %s",
                     status::code_to_str(code));
        }

        std::string builtin_password = device_info.get_fw_name();
        builtin_password += "-";
        builtin_password += std::string(device_info.get_device_id(), 7);

        strncpy(password_, builtin_password.c_str(), sizeof(password_));
    }
}

const char* ApNetworkConfig::get_ssid() const {
    return ssid_;
}

const char* ApNetworkConfig::get_password() const {
    return password_;
}

uint8_t ApNetworkConfig::get_channel() const {
    return CONFIG_OCS_NETWORK_WIFI_AP_CHANNEL;
}

uint8_t ApNetworkConfig::get_max_conn() const {
    return CONFIG_OCS_NETWORK_WIFI_AP_MAX_CONN;
}

} // namespace config
} // namespace pipeline
} // namespace ocs
