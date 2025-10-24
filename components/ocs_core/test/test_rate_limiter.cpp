/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "unity.h"

#include "ocs_core/rate_limiter.h"
#include "ocs_test/test_clock.h"

namespace ocs {
namespace core {

TEST_CASE("Rate limiter: allow operation to be performed", "[ocs_core], [rate_limiter]") {
    const system::Time interval = system::Duration::second;

    test::TestClock clock;
    clock.value = 42;

    RateLimiter limiter(clock, interval);

    // Limiter has just been created and should be allowed to operate.
    TEST_ASSERT_TRUE(limiter.allow());

    // Operation has been performed, no more than one operation per interval is allowed.
    TEST_ASSERT_FALSE(limiter.allow());

    // Update the current time, but do not exceed the rate interval.
    clock.value += interval - system::Duration::microsecond;
    TEST_ASSERT_FALSE(limiter.allow());

    // Again, but now the rate interval has been exceeded.
    clock.value += system::Duration::microsecond;
    TEST_ASSERT_TRUE(limiter.allow());
    TEST_ASSERT_FALSE(limiter.allow());
}

} // namespace core
} // namespace ocs
