/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_algo/storage_ops.h"
#include "ocs_core/log.h"
#include "ocs_pipeline/config/mdns_config.h"

namespace ocs {
namespace pipeline {
namespace config {

namespace {

const char* log_tag = "mdns_config";

} // namespace

MdnsConfig::MdnsConfig(const system::DeviceInfo& device_info,
                       storage::StorageBuilder& storage_builder) {
    storage_ = storage_builder.make("mdns_config");
    configASSERT(storage_);

    memset(hostname_, 0, sizeof(hostname_));
    auto code = algo::StorageOps::prob_read(*storage_, hostname_key_, hostname_,
                                            max_hostname_size_);
    if (code != status::StatusCode::OK) {
        memcpy(hostname_, device_info.get_fw_name(),
               std::min(max_hostname_size_, strlen(device_info.get_fw_name())));
    }

    memset(instance_name_, 0, sizeof(instance_name_));
    code = algo::StorageOps::prob_read(*storage_, instance_name_key_, instance_name_,
                                       max_instance_name_size_);
    if (code != status::StatusCode::OK) {
        memcpy(
            instance_name_, device_info.get_fw_description(),
            std::min(max_instance_name_size_, strlen(device_info.get_fw_description())));
    }
}

const char* MdnsConfig::get_hostname() const {
    return hostname_;
}

const char* MdnsConfig::get_instance_name() const {
    return instance_name_;
}

status::StatusCode MdnsConfig::configure(const char* hostname,
                                         const char* instance_name) {
    if (strlen(hostname) > max_hostname_size_) {
        return status::StatusCode::InvalidArg;
    }

    if (strlen(instance_name) > max_instance_name_size_) {
        return status::StatusCode::InvalidArg;
    }

    bool modified = false;

    if (strncmp(hostname_, hostname, std::min(strlen(hostname_), strlen(hostname)))) {
        const auto code = storage_->write(hostname_key_, hostname, strlen(hostname));
        if (code != status::StatusCode::OK) {
            return code;
        }

        modified = true;
    }

    if (strncmp(instance_name_, instance_name,
                std::min(strlen(instance_name_), strlen(instance_name)))) {
        const auto code =
            storage_->write(instance_name_key_, instance_name, strlen(instance_name));
        if (code != status::StatusCode::OK) {
            return code;
        }

        modified = true;
    }

    if (modified) {
        ocs_logi(log_tag, "configuration modified: old=(%s:%s) new=(%s:%s)", hostname_,
                 instance_name_, hostname, instance_name);

        return status::StatusCode::OK;
    }

    return status::StatusCode::NotModified;
}

} // namespace config
} // namespace pipeline
} // namespace ocs
