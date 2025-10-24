/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <algorithm>
#include <cstring>

#include "ocs_algo/storage_ops.h"
#include "ocs_core/log.h"
#include "ocs_net/sta_network_config.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace net {

namespace {

const char* log_tag = "sta_network_config";

} // namespace

StaNetworkConfig::StaNetworkConfig(storage::IStorage& storage)
    : storage_(storage) {
    memset(ssid_, 0, sizeof(ssid_));
    memset(password_, 0, sizeof(password_));

    auto code = algo::StorageOps::prob_read(storage_, max_retry_count_key_,
                                            &max_retry_count_, sizeof(max_retry_count_));
    if (code != status::StatusCode::OK) {
        if (code != status::StatusCode::NoData) {
            ocs_loge(log_tag, "failed to read WiFi STA max retry count from storage: %s",
                     status::code_to_str(code));
        }

        max_retry_count_ = default_max_retry_count_;
    }

    code =
        algo::StorageOps::prob_read(storage_, password_key_, password_, max_password_len);
    if (code != status::StatusCode::OK) {
        if (code != status::StatusCode::NoData) {
            ocs_loge(log_tag, "failed to read WiFi STA password from storage: %s",
                     status::code_to_str(code));
        }
    }

    code = algo::StorageOps::prob_read(storage_, ssid_key_, ssid_, max_ssid_len);
    if (code != status::StatusCode::OK) {
        if (code != status::StatusCode::NoData) {
            ocs_loge(log_tag, "failed to read WiFi STA SSID from storage: %s",
                     status::code_to_str(code));
        }
    }
}

bool StaNetworkConfig::valid() const {
    return strlen(ssid_) >= min_ssid_len && strlen(password_) >= min_password_len;
}

uint8_t StaNetworkConfig::get_max_retry_count() const {
    return max_retry_count_;
}

const char* StaNetworkConfig::get_ssid() const {
    return ssid_;
}

const char* StaNetworkConfig::get_password() const {
    return password_;
}

status::StatusCode StaNetworkConfig::configure(uint8_t max_retry_count,
                                               const char* ssid,
                                               const char* password) {
    if (max_retry_count < min_max_retry_count || max_retry_count > max_max_retry_count) {
        return status::StatusCode::InvalidArg;
    }

    if (strlen(ssid) < min_ssid_len || strlen(ssid) > max_ssid_len) {
        return status::StatusCode::InvalidArg;
    }

    if (strlen(password) < min_password_len || strlen(password) > max_password_len) {
        return status::StatusCode::InvalidArg;
    }

    bool modified = false;

    if (max_retry_count_ != max_retry_count) {
        const auto code = storage_.write(max_retry_count_key_, &max_retry_count,
                                         sizeof(max_retry_count));
        if (code != status::StatusCode::OK) {
            return code;
        }

        modified = true;
    }

    if (!valid() || strncmp(ssid_, ssid, std::min(strlen(ssid_), strlen(ssid)))) {
        const auto code = storage_.write(ssid_key_, ssid, strlen(ssid));
        if (code != status::StatusCode::OK) {
            return code;
        }

        modified = true;
    }

    if (!valid()
        || strncmp(password_, password, std::min(strlen(password_), strlen(password)))) {
        const auto code = storage_.write(password_key_, password, strlen(password));
        if (code != status::StatusCode::OK) {
            return code;
        }

        modified = true;
    }

    return modified ? status::StatusCode::OK : status::StatusCode::NotModified;
}

status::StatusCode StaNetworkConfig::reset() {
    auto code = storage_.erase_all();
    if (code == status::StatusCode::NoData) {
        code = status::StatusCode::NotModified;
    }

    return code;
}

} // namespace net
} // namespace ocs
