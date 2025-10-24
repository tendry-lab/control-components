/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_pipeline/jsonfmt/locate_formatter.h"
#include "ocs_fmt/json/cjson_object_formatter.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

LocateFormatter::LocateFormatter(control::ILocator& locator)
    : locator_(locator) {
}

status::StatusCode LocateFormatter::format(cJSON* json) {
    fmt::json::CjsonObjectFormatter formatter(json);

    if (!formatter.add_bool_cs("locate", locator_.get())) {
        return status::StatusCode::NoMem;
    }

    return status::StatusCode::OK;
}

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
