/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_system.h"

#include "ocs_fmt/json/cjson_object_formatter.h"
#include "ocs_pipeline/jsonfmt/toolchain_formatter.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

status::StatusCode ToolchainFormatter::format(cJSON* json) {
    fmt::json::CjsonObjectFormatter formatter(json);

    if (!formatter.add_string_ref_cs("toolchain_name", "ESP-IDF")) {
        return status::StatusCode::NoMem;
    }
    if (!formatter.add_string_ref_cs("toolchain_version", esp_get_idf_version())) {
        return status::StatusCode::NoMem;
    }

    return status::StatusCode::OK;
}

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
