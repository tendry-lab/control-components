/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_fmt/json/fanout_formatter.h"

namespace ocs {
namespace fmt {
namespace json {

status::StatusCode FanoutFormatter::format(cJSON* json) {
    for (auto& formatter : formatters_) {
        const auto code = formatter->format(json);
        if (code != status::StatusCode::OK) {
            return code;
        }
    }

    return status::StatusCode::OK;
}

void FanoutFormatter::add(IFormatter& formatter) {
    formatters_.emplace_back(&formatter);
}

} // namespace json
} // namespace fmt
} // namespace ocs
