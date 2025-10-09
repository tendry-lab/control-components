/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_algo/storage_ops.h"
#include "ocs_core/log.h"
#include "ocs_net/mdns_config.h"

namespace ocs {
namespace net {

namespace {

const char* log_tag = "mdns_config";

} // namespace

MdnsConfig::MdnsConfig(storage::IStorage& storage, const system::DeviceInfo& device_info)
    : storage_(storage) {
    memset(hostname_, 0, sizeof(hostname_));
    auto code =
        algo::StorageOps::prob_read(storage_, hostname_key_, hostname_, max_hostname_len);
    if (code != status::StatusCode::OK) {
        memcpy(hostname_, device_info.get_fw_name(),
               std::min(max_hostname_len, strlen(device_info.get_fw_name())));
    }
}

status::StatusCode MdnsConfig::reset() {
    auto code = storage_.erase(hostname_key_);
    if (code == status::StatusCode::NoData) {
        code = status::StatusCode::NotModified;
    }

    return code;
}

const char* MdnsConfig::get_hostname() const {
    return hostname_;
}

status::StatusCode MdnsConfig::configure(const char* hostname) {
    if (strlen(hostname) > max_hostname_len) {
        return status::StatusCode::InvalidArg;
    }

    bool modified = false;

    if (strncmp(hostname_, hostname, std::min(strlen(hostname_), strlen(hostname)))) {
        const auto code = storage_.write(hostname_key_, hostname, strlen(hostname));
        if (code != status::StatusCode::OK) {
            return code;
        }

        modified = true;
    }

    if (modified) {
        ocs_logi(log_tag, "configuration modified: old=%s new=%s", hostname_, hostname);

        return status::StatusCode::OK;
    }

    return status::StatusCode::NotModified;
}

} // namespace net
} // namespace ocs
