/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <ctime>
#include <optional>

#include "ocs_status/code.h"
#include "ocs_system/time.h"

namespace ocs {
namespace algo {

struct TimeOps {
    //! Return true if @p interval is passed since @p start time.
    template <typename T> static bool after(T start, T now, T interval) {
        const T dest = start + interval;

        if (now < start) {
            if (dest > start) {
                return true;
            }

            if (dest <= now) {
                return true;
            }

            return false;
        }

        if (dest > now) {
            return false;
        }

        if (dest < start) {
            return false;
        }

        return true;
    }

    //! Return system time.
    static std::optional<time_t> get_time();

    //! Parse system time from @p str.
    static std::optional<time_t> parse_time(const char* str);

    //! Set the system time from @p str, making sure that the set time is greater
    //! than @p start_point.
    static status::StatusCode set_time(const char* str, time_t start_point);
};

} // namespace algo
} // namespace ocs
