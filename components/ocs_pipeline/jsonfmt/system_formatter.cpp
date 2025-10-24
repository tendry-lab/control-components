/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_system.h"

#include "ocs_fmt/json/cjson_object_formatter.h"
#include "ocs_pipeline/jsonfmt/system_formatter.h"
#include "ocs_system/reset_reason_to_str.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

status::StatusCode SystemFormatter::format(cJSON* json) {
    fmt::json::CjsonObjectFormatter formatter(json);

    if (!formatter.add_number_cs("system_memory_heap", esp_get_free_heap_size())) {
        return status::StatusCode::NoMem;
    }

    if (!formatter.add_number_cs("system_memory_heap_min",
                                 esp_get_minimum_free_heap_size())) {
        return status::StatusCode::NoMem;
    }

    if (!formatter.add_number_cs("system_memory_heap_internal",
                                 esp_get_free_internal_heap_size())) {
        return status::StatusCode::NoMem;
    }

    if (!formatter.add_string_ref_cs("system_reset_reason",
                                     system::reset_reason_to_str(esp_reset_reason()))) {
        return status::StatusCode::NoMem;
    }

    return status::StatusCode::OK;
}

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
