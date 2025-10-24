/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstring>

#include "ocs_algo/string_ops.h"

namespace ocs {
namespace algo {

StringOps::Values StringOps::split(char delimiter, const StringOps::Value& value) {
    if (value.size() < 2) {
        return {};
    }

    const char* start = value.data();

    Values values;

    while (true) {
        // Check if last symbol.
        if (start == &value.back()) {
            if (*start != delimiter) {
                values.emplace_back(Value(start, 1));
            }
            break;
        }

        const char* end = strchr(start, delimiter);
        if (!end) {
            // Make sure that the string contains the required delimiter.
            if (values.size()) {
                values.emplace_back(Value(start, value.data() + value.size() - start));
            }
            break;
        }

        if (*start != delimiter) {
            values.emplace_back(Value(start, end - start));
        }

        // Check if the delimiter was a last symbol.
        if (end == &value.back()) {
            break;
        }

        start = end;
        ++start;
    }

    return values;
}

} // namespace algo
} // namespace ocs
