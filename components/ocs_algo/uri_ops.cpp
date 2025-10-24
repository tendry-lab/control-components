/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_algo/uri_ops.h"

namespace ocs {
namespace algo {

namespace {

void parse_key_value(UriOps::Values& values, const char* begin, const char* end) {
    configASSERT(begin);
    configASSERT(end);

    const char* delimiter = strchr(begin, '=');
    if (!delimiter || delimiter == begin || delimiter == end) {
        return;
    }

    const std::string_view key(begin, delimiter - begin);
    const std::string_view value(delimiter + 1, end - delimiter);

    values.emplace(key, value);
}

} // namespace

UriOps::Path UriOps::parse_path(const char* uri) {
    if (!uri) {
        return std::string_view();
    }

    unsigned path_length = 0;

    const char* query_start = strchr(uri, '?');
    if (query_start != nullptr) {
        path_length = query_start - uri;
    } else {
        path_length = strlen(uri);
    }

    return std::string_view(uri, path_length);
}

UriOps::Values UriOps::parse_query(const char* uri) {
    if (!uri) {
        return {};
    }

    const char* query_start = strchr(uri, '?');
    if (!query_start) {
        return {};
    }

    Values values;

    while (true) {
        // Check if last symbol.
        if (query_start - uri == strlen(uri)) {
            break;
        }

        ++query_start;

        const char* end = strchr(query_start, '&');
        if (!end) {
            parse_key_value(values, query_start, query_start + strlen(query_start) - 1);
            break;
        }

        parse_key_value(values, query_start, end - 1);
        query_start = end;
    }

    return values;
}

} // namespace algo
} // namespace ocs
