/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_pipeline/jsonfmt/counter_formatter.h"
#include "ocs_fmt/json/cjson_object_formatter.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

CounterFormatter::CounterFormatter(diagnostic::CounterStore& store)
    : store_(store) {
}

status::StatusCode CounterFormatter::format(cJSON* json) {
    fmt::json::CjsonObjectFormatter formatter(json);

    for (auto& counter : store_.get()) {
        if (!formatter.add_number_cs(counter->id(), counter->get())) {
            return status::StatusCode::NoMem;
        }
    }

    return status::StatusCode::OK;
}

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
