/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_pipeline/jsonfmt/counter_formatter.h"
#include "ocs_fmt/json/cjson_object_formatter.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

status::StatusCode CounterFormatter::format(cJSON* json) {
    fmt::json::CjsonObjectFormatter formatter(json);

    for (auto& counter : get_counters_()) {
        if (!formatter.add_number_cs(counter->id(), counter->get())) {
            return status::StatusCode::NoMem;
        }
    }

    return status::StatusCode::OK;
}

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
