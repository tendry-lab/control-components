/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/log.h"
#include "ocs_core/version.h"
#include "ocs_pipeline/jsonfmt/registration_formatter.h"
#include "ocs_pipeline/jsonfmt/toolchain_formatter.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

namespace {

const char* log_tag = "registration_json_formatter";

} // namespace

RegistrationFormatter::RegistrationFormatter(const system::DeviceInfo& device_info) {
    fanout_formatter_.reset(new (std::nothrow) fmt::json::FanoutFormatter());
    configASSERT(fanout_formatter_);

    toolchain_formatter_.reset(new (std::nothrow) ToolchainFormatter());
    configASSERT(toolchain_formatter_);
    fanout_formatter_->add(*toolchain_formatter_);

    string_formatter_.reset(new (std::nothrow) fmt::json::StringFormatter());
    configASSERT(string_formatter_);
    fanout_formatter_->add(*string_formatter_);

    core::Version version;
    if (version.parse(device_info.get_fw_version())) {
        string_formatter_->add("fw_version", device_info.get_fw_version());
    } else {
        ocs_loge(log_tag, "failed to parse FW version: %s", device_info.get_fw_version());
        string_formatter_->add("fw_version", "<none>");
    }

    string_formatter_->add("fw_name", device_info.get_fw_name());
    string_formatter_->add("fw_description", device_info.get_fw_description());
    string_formatter_->add("device_id", device_info.get_device_id());
    string_formatter_->add("product_name", device_info.get_product_name());
}

status::StatusCode RegistrationFormatter::format(cJSON* json) {
    return fanout_formatter_->format(json);
}

fmt::json::FanoutFormatter& RegistrationFormatter::get_fanout_formatter() {
    return *fanout_formatter_;
}

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
