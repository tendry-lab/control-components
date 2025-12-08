/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstdio>

#include "ocs_core/version_to_str.h"

namespace ocs {
namespace core {

version_to_str::version_to_str(Version version) {
    memset(buf_, 0, sizeof(buf_));

    if (snprintf(buf_, sizeof(buf_), "%u.%u.%u", static_cast<size_t>(version.major),
                 static_cast<size_t>(version.minor), static_cast<size_t>(version.patch))
        < 0) {
        strcpy(buf_, "<none>");
    }
}

const char* version_to_str::c_str() const {
    return buf_;
}

} // namespace core
} // namespace ocs
