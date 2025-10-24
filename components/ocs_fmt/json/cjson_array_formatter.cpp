/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_fmt/json/cjson_array_formatter.h"
#include "ocs_fmt/json/cjson_builder.h"

namespace ocs {
namespace fmt {
namespace json {

CjsonArrayFormatter::CjsonArrayFormatter(cJSON* json)
    : json_(json) {
    configASSERT(cJSON_IsArray(json_));
}

bool CjsonArrayFormatter::append_uint16(uint16_t value) {
    auto item = CjsonUniqueBuilder::make_number(value);
    if (!item) {
        return false;
    }

    if (!cJSON_AddItemToArray(json_, item.get())) {
        return false;
    }

    item.release();
    return true;
}

bool CjsonArrayFormatter::append_string(const char* str) {
    auto item = CjsonUniqueBuilder::make_string(str);
    if (!item) {
        return false;
    }

    if (!cJSON_AddItemToArray(json_, item.get())) {
        return false;
    }

    item.release();
    return true;
}

bool CjsonArrayFormatter::append_string_ref(const char* str) {
    auto item = CjsonUniqueBuilder::make_string_ref(str);
    if (!item) {
        return false;
    }

    if (!cJSON_AddItemToArray(json_, item.get())) {
        return false;
    }

    item.release();
    return true;
}

} // namespace json
} // namespace fmt
} // namespace ocs
