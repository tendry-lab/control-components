/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
