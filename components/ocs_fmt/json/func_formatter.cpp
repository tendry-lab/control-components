/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_fmt/json/func_formatter.h"

namespace ocs {
namespace fmt {
namespace json {

FuncFormatter::FuncFormatter(FuncFormatter::Func func)
    : func_(func) {
}

status::StatusCode FuncFormatter::format(cJSON* json) {
    return func_(json);
}

} // namespace json
} // namespace fmt
} // namespace ocs
