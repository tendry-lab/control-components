/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <sys/time.h>

#include "ocs_algo/time_ops.h"
#include "ocs_core/log.h"

namespace ocs {
namespace algo {

namespace {

const char* log_tag = "time_ops";

} // namespace

std::optional<time_t> TimeOps::get_time() {
    const time_t timestamp = time(nullptr);
    if (timestamp == -1) {
        ocs_loge(log_tag, "time(): %s", std::strerror(errno));

        return std::nullopt;
    }

    return timestamp;
}

std::optional<time_t> TimeOps::parse_time(const char* str) {
    if (!str || *str == '\0') {
        return std::nullopt;
    }

    char* end = nullptr;
    errno = 0;
    const auto value = strtol(str, &end, 10);

    if (end == str || *end != '\0') {
        return std::nullopt;
    }

    if (errno == ERANGE || value < 0 || value > std::numeric_limits<time_t>::max()) {
        return std::nullopt;
    }

    return value;
}

status::StatusCode TimeOps::set_time(const char* str, time_t start_point) {
    const auto timestamp = parse_time(str);
    if (!timestamp) {
        return status::StatusCode::InvalidArg;
    }

    if (*timestamp < start_point) {
        return status::StatusCode::InvalidArg;
    }

    const timeval time_value = { *timestamp, 0 };
    if (settimeofday(&time_value, nullptr) == -1) {
        ocs_loge(log_tag, "settimeofday(): %s", std::strerror(errno));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

} // namespace algo
} // namespace ocs
