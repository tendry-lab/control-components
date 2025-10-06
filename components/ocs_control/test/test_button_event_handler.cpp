/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_control/button_event_handler.h"
#include "ocs_test/test_clock.h"

namespace ocs {
namespace control {

namespace {

struct TestButton : public IButton, private core::NonCopyable<> {
    bool pressed { false };

    bool get() override {
        return pressed;
    }
};

struct TestButtonHandler : public IButtonHandler, private core::NonCopyable<> {
    core::Time pressed_duration { 0 };
    unsigned pressed_call_count { 0 };

    status::StatusCode handle_pressed(core::Time duration) {
        pressed_duration = duration;
        ++pressed_call_count;

        return status::StatusCode::OK;
    }
};

} // namespace

TEST_CASE("Button event handler: pressed/released",
          "[button_event_handler], [ocs_control]") {
    test::TestClock clock;
    TestButton button;
    TestButtonHandler button_handler;

    ButtonEventHandler event_handler(clock, button, button_handler);
    TEST_ASSERT_EQUAL(0, button_handler.pressed_duration);
    TEST_ASSERT_EQUAL(0, button_handler.pressed_call_count);

    clock.value = core::Duration::second * 10;
    TEST_ASSERT_EQUAL(status::StatusCode::OK, event_handler.handle_event());
    TEST_ASSERT_EQUAL(0, button_handler.pressed_duration);
    TEST_ASSERT_EQUAL(0, button_handler.pressed_call_count);

    button.pressed = true;
    TEST_ASSERT_EQUAL(status::StatusCode::OK, event_handler.handle_event());
    TEST_ASSERT_EQUAL(0, button_handler.pressed_duration);
    TEST_ASSERT_EQUAL(0, button_handler.pressed_call_count);

    clock.value += core::Duration::second;

    button.pressed = true;
    TEST_ASSERT_EQUAL(status::StatusCode::OK, event_handler.handle_event());
    TEST_ASSERT_EQUAL(0, button_handler.pressed_duration);
    TEST_ASSERT_EQUAL(0, button_handler.pressed_call_count);

    clock.value += core::Duration::second;

    button.pressed = false;
    TEST_ASSERT_EQUAL(status::StatusCode::OK, event_handler.handle_event());
    TEST_ASSERT_EQUAL(2 * core::Duration::second, button_handler.pressed_duration);
    TEST_ASSERT_EQUAL(1, button_handler.pressed_call_count);

    button.pressed = false;
    TEST_ASSERT_EQUAL(status::StatusCode::OK, event_handler.handle_event());
    TEST_ASSERT_EQUAL(2 * core::Duration::second, button_handler.pressed_duration);
    TEST_ASSERT_EQUAL(1, button_handler.pressed_call_count);

    button_handler.pressed_duration = 0;

    button.pressed = true;
    TEST_ASSERT_EQUAL(status::StatusCode::OK, event_handler.handle_event());
    TEST_ASSERT_EQUAL(0, button_handler.pressed_duration);
    TEST_ASSERT_EQUAL(1, button_handler.pressed_call_count);

    clock.value += core::Duration::second;

    button.pressed = true;
    TEST_ASSERT_EQUAL(status::StatusCode::OK, event_handler.handle_event());
    TEST_ASSERT_EQUAL(0, button_handler.pressed_duration);
    TEST_ASSERT_EQUAL(1, button_handler.pressed_call_count);

    clock.value += 2 * core::Duration::second;

    button.pressed = false;
    TEST_ASSERT_EQUAL(status::StatusCode::OK, event_handler.handle_event());
    TEST_ASSERT_EQUAL(3 * core::Duration::second, button_handler.pressed_duration);
    TEST_ASSERT_EQUAL(2, button_handler.pressed_call_count);
}

} // namespace control
} // namespace ocs
