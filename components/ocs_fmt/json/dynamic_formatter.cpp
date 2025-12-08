/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_fmt/json/dynamic_formatter.h"

namespace ocs {
namespace fmt {
namespace json {

DynamicFormatter::DynamicFormatter(size_t size)
    : size_(size) {
    configASSERT(size_);

    buf_.reset(new (std::nothrow) char[size_]);
    configASSERT(buf_);
}

status::StatusCode DynamicFormatter::format(cJSON* json) {
    clear_();

    if (!cJSON_PrintPreallocated(json, buf_.get(), size_, false)) {
        return status::StatusCode::NoMem;
    }

    return status::StatusCode::OK;
}

const char* DynamicFormatter::c_str() const {
    return buf_.get();
}

void DynamicFormatter::clear_() {
    memset(buf_.get(), 0, size_);
}

} // namespace json
} // namespace fmt
} // namespace ocs
