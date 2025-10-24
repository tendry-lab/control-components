/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_algo/time_ops.h"
#include "ocs_core/rate_limiter.h"

namespace ocs {
namespace core {

RateLimiter::RateLimiter(system::IClock& clock, system::Time interval)
    : interval_(interval)
    , clock_(clock) {
    configASSERT(interval > 0);
}

bool RateLimiter::allow() {
    const auto now = clock_.now();

    if (start_) {
        if (!algo::TimeOps::after(start_, now, interval_)) {
            return false;
        }
    }

    start_ = now;
    return true;
}

} // namespace core
} // namespace ocs
