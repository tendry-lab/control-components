/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_control/gpio_led.h"
#include "ocs_test/test_gpio.h"

namespace ocs {
namespace control {

TEST_CASE("GPIO led: lock/unlock", "[ocs_core], [gpio_led]") {
    test::TestGpio gpio(status::StatusCode::OK, status::StatusCode::OK,
                        status::StatusCode::OK);
    GpioLed led(gpio);

    TEST_ASSERT_EQUAL(status::StatusCode::InvalidState, led.turn_on());
    TEST_ASSERT_EQUAL(status::StatusCode::InvalidState, led.turn_off());
    TEST_ASSERT_EQUAL(status::StatusCode::InvalidState, led.flip());

    TEST_ASSERT_EQUAL(status::StatusCode::OK, led.try_lock(ILed::Priority::Locate));
    TEST_ASSERT_EQUAL(status::StatusCode::OK, led.try_lock(ILed::Priority::Locate));

    TEST_ASSERT_EQUAL(status::StatusCode::OK, led.try_lock(ILed::Priority::System));

    TEST_ASSERT_EQUAL(status::StatusCode::InvalidArg,
                      led.try_lock(ILed::Priority::FatalError));

    TEST_ASSERT_EQUAL(status::StatusCode::InvalidArg,
                      led.try_lock(ILed::Priority::Default));

    TEST_ASSERT_EQUAL(status::StatusCode::OK, led.turn_on());
    TEST_ASSERT_TRUE(gpio.get());
    TEST_ASSERT_EQUAL(1, gpio.turn_on_call_count);
    TEST_ASSERT_EQUAL(0, gpio.turn_off_call_count);
    TEST_ASSERT_EQUAL(0, gpio.flip_call_count);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, led.turn_off());
    TEST_ASSERT_FALSE(gpio.get());
    TEST_ASSERT_EQUAL(1, gpio.turn_on_call_count);
    TEST_ASSERT_EQUAL(1, gpio.turn_off_call_count);
    TEST_ASSERT_EQUAL(0, gpio.flip_call_count);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, led.flip());
    TEST_ASSERT_TRUE(gpio.get());
    TEST_ASSERT_EQUAL(1, gpio.turn_on_call_count);
    TEST_ASSERT_EQUAL(1, gpio.turn_off_call_count);
    TEST_ASSERT_EQUAL(1, gpio.flip_call_count);

    TEST_ASSERT_EQUAL(status::StatusCode::InvalidArg,
                      led.try_unlock(ILed::Priority::Default));

    TEST_ASSERT_EQUAL(status::StatusCode::OK, led.try_unlock(ILed::Priority::System));

    TEST_ASSERT_EQUAL(status::StatusCode::InvalidState, led.turn_on());
    TEST_ASSERT_EQUAL(status::StatusCode::InvalidState, led.turn_off());
    TEST_ASSERT_EQUAL(status::StatusCode::InvalidState, led.flip());
    TEST_ASSERT_TRUE(gpio.get());
    TEST_ASSERT_EQUAL(1, gpio.turn_on_call_count);
    TEST_ASSERT_EQUAL(1, gpio.turn_off_call_count);
    TEST_ASSERT_EQUAL(1, gpio.flip_call_count);
}

} // namespace control
} // namespace ocs
