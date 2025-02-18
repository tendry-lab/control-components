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
#include "ocs_net/ap_network_config.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace net {

namespace {

const char* log_tag = "ap_network_config";

} // namespace

ApNetworkConfig::ApNetworkConfig(storage::IStorage& storage,
                                 const system::DeviceInfo& device_info)
    : storage_(storage) {
    memset(ssid_, 0, sizeof(ssid_));
    memset(password_, 0, sizeof(password_));

    std::string builtin_ssid = device_info.get_fw_name();
    builtin_ssid += "-";
    builtin_ssid += device_info.get_device_id();

    strncpy(ssid_, builtin_ssid.c_str(), sizeof(ssid_));

    auto code =
        algo::StorageOps::prob_read(storage_, password_key_, password_, max_password_len);
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

    code =
        algo::StorageOps::prob_read(storage_, channel_key_, &channel_, sizeof(channel_));
    if (code != status::StatusCode::OK) {
        if (code != status::StatusCode::NoData) {
            ocs_loge(log_tag, "failed to read WiFi AP channel from storage: %s",
                     status::code_to_str(code));
        }

        channel_ = CONFIG_OCS_NETWORK_WIFI_AP_CHANNEL;
    }

    code = algo::StorageOps::prob_read(storage_, max_conn_key_, &max_conn_,
                                       sizeof(max_conn_));
    if (code != status::StatusCode::OK) {
        if (code != status::StatusCode::NoData) {
            ocs_loge(log_tag, "failed to read WiFi AP max connection from storage: %s",
                     status::code_to_str(code));
        }

        max_conn_ = CONFIG_OCS_NETWORK_WIFI_AP_MAX_CONN;
    }
}

const char* ApNetworkConfig::get_ssid() const {
    return ssid_;
}

const char* ApNetworkConfig::get_password() const {
    return password_;
}

uint8_t ApNetworkConfig::get_channel() const {
    return channel_;
}

uint8_t ApNetworkConfig::get_max_conn() const {
    return max_conn_;
}

status::StatusCode
ApNetworkConfig::configure(uint8_t channel, uint8_t max_conn, const char* password) {
    if (channel < min_channel || channel > max_channel) {
        return status::StatusCode::InvalidArg;
    }

    if (max_conn < min_max_conn || max_conn > max_max_conn) {
        return status::StatusCode::InvalidArg;
    }

    if (strlen(password) < min_password_len || strlen(password) > max_password_len) {
        return status::StatusCode::InvalidArg;
    }

    bool modified = false;

    if (strncmp(password_, password, std::min(strlen(password_), strlen(password)))) {
        const auto code = storage_.write(password_key_, password, strlen(password));
        if (code != status::StatusCode::OK) {
            return code;
        }

        modified = true;
    }

    if (channel_ != channel) {
        const auto code = storage_.write(channel_key_, &channel, sizeof(channel));
        if (code != status::StatusCode::OK) {
            return code;
        }

        modified = true;
    }

    if (max_conn_ != max_conn) {
        const auto code = storage_.write(max_conn_key_, &max_conn, sizeof(max_conn));
        if (code != status::StatusCode::OK) {
            return code;
        }

        modified = true;
    }

    return modified ? status::StatusCode::OK : status::StatusCode::NotModified;
}

status::StatusCode ApNetworkConfig::reset() {
    auto code = storage_.erase_all();
    if (code == status::StatusCode::NoData) {
        code = status::StatusCode::NotModified;
    }

    return code;
}

} // namespace net
} // namespace ocs
