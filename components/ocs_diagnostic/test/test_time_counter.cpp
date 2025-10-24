/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "unity.h"

#include "ocs_diagnostic/time_counter.h"
#include "ocs_test/test_clock.h"

namespace ocs {
namespace diagnostic {

TEST_CASE("Time counter: gradually reach resolution",
          "[ocs_diagnostic], [time_counter]") {
    test::TestClock clock;
    clock.value = system::Duration::millisecond * 500;

    TimeCounter counter(clock, "counter", system::Duration::second);
    TEST_ASSERT_EQUAL(0, counter.get());

    clock.value += system::Duration::millisecond * 499;
    TEST_ASSERT_EQUAL(0, counter.get());

    clock.value += system::Duration::millisecond;
    TEST_ASSERT_EQUAL(1, counter.get());
}

TEST_CASE("Time counter: reach resolution after reset",
          "[ocs_diagnostic], [time_counter]") {
    test::TestClock clock;
    clock.value = system::Duration::millisecond * 500;

    TimeCounter counter(clock, "counter", system::Duration::second);

    TEST_ASSERT_EQUAL(0, counter.get());

    clock.value += system::Duration::millisecond * 499;
    TEST_ASSERT_EQUAL(0, counter.get());

    counter.reset();
    TEST_ASSERT_EQUAL(0, counter.get());

    clock.value += system::Duration::millisecond * 999;
    TEST_ASSERT_EQUAL(0, counter.get());

    clock.value += system::Duration::millisecond;
    TEST_ASSERT_EQUAL(1, counter.get());
}

} // namespace diagnostic
} // namespace ocs
