/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_control/gpio_led.h"
#include "ocs_control/system_fsm.h"
#include "ocs_core/noncopyable.h"
#include "ocs_scheduler/constant_delay_estimator.h"
#include "ocs_scheduler/periodic_task_scheduler.h"
#include "ocs_test/test_clock.h"
#include "ocs_test/test_gpio.h"

namespace ocs {
namespace control {

namespace {

struct TestRebooter : public system::IRebooter, private core::NonCopyable<> {
    unsigned call_count { 0 };

    void reboot() override {
        ++call_count;
    }
};

} // namespace

TEST_CASE("System FSM: button is pressed before LED reaction on system initialization",
          "[system_fsm], [ocs_control]") {
    const core::Time want_release_interval = core::Duration::second;
    const core::Time got_release_interval = want_release_interval / 2;

    test::TestClock clock;
    scheduler::ConstantDelayEstimator estimator(pdMS_TO_TICKS(10));
    scheduler::PeriodicTaskScheduler task_scheduler(clock, estimator, "scheduler", 16);

    test::TestGpio gpio(status::StatusCode::OK, status::StatusCode::OK,
                        status::StatusCode::OK);
    GpioLED led(gpio);

    TestRebooter rebooter;

    SystemFsm fsm(rebooter, task_scheduler, led, want_release_interval);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.handle_pressed(got_release_interval));
    taskYIELD();

    // Start the LED reaction on system initialization.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());

    // Wait for the LED reaction on system initialization.
    while (gpio.flip_call_count != 6) {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    }
    gpio.flip_call_count = 0;

    // Handle button event.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());

    // Wait for the LED reaction on button event.
    while (gpio.flip_call_count != 2) {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    }
    gpio.flip_call_count = 0;

    TEST_ASSERT_EQUAL(1, rebooter.call_count);
}

TEST_CASE("System FSM: button is pressed after LED reaction on system initialization",
          "[system_fsm], [ocs_control]") {
    const core::Time want_release_interval = core::Duration::second;
    const core::Time got_release_interval = want_release_interval / 2;

    test::TestClock clock;
    scheduler::ConstantDelayEstimator estimator(pdMS_TO_TICKS(10));
    scheduler::PeriodicTaskScheduler task_scheduler(clock, estimator, "scheduler", 16);

    test::TestGpio gpio(status::StatusCode::OK, status::StatusCode::OK,
                        status::StatusCode::OK);
    GpioLED led(gpio);

    TestRebooter rebooter;

    SystemFsm fsm(rebooter, task_scheduler, led, want_release_interval);

    // Start LED reaction on system initialization.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());

    while (gpio.flip_call_count != 6) {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    }
    gpio.flip_call_count = 0;

    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.handle_pressed(got_release_interval));
    vTaskDelay(pdMS_TO_TICKS(10));

    // Handle button event.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());

    // Wait for button LED indication.
    while (gpio.flip_call_count != 2) {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    }
    gpio.flip_call_count = 0;

    TEST_ASSERT_EQUAL(1, rebooter.call_count);
}

TEST_CASE("System FSM: button is pressed during LED reaction on system initialization",
          "[system_fsm], [ocs_control]") {
    const core::Time want_release_interval = core::Duration::second;
    const core::Time got_release_interval = want_release_interval / 2;

    test::TestClock clock;
    scheduler::ConstantDelayEstimator estimator(pdMS_TO_TICKS(10));
    scheduler::PeriodicTaskScheduler task_scheduler(clock, estimator, "scheduler", 16);

    test::TestGpio gpio(status::StatusCode::OK, status::StatusCode::OK,
                        status::StatusCode::OK);
    GpioLED led(gpio);

    TestRebooter rebooter;

    SystemFsm fsm(rebooter, task_scheduler, led, want_release_interval);

    // Start LED reaction on system initialization.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());

    while (gpio.flip_call_count != 2) {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    }

    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.handle_pressed(got_release_interval));
    vTaskDelay(pdMS_TO_TICKS(10));

    while (gpio.flip_call_count != 6) {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    }
    gpio.flip_call_count = 0;

    // Handle button event.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());

    // Wait for button LED indication.
    while (gpio.flip_call_count != 2) {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    }
    gpio.flip_call_count = 0;

    TEST_ASSERT_EQUAL(1, rebooter.call_count);
}

TEST_CASE("System FSM: button isn't released within interval",
          "[system_fsm], [ocs_control]") {
    const core::Time want_release_interval = core::Duration::second;
    const core::Time got_release_interval = want_release_interval * 2;

    test::TestClock clock;
    scheduler::ConstantDelayEstimator estimator(pdMS_TO_TICKS(10));
    scheduler::PeriodicTaskScheduler task_scheduler(clock, estimator, "scheduler", 16);

    test::TestGpio gpio(status::StatusCode::OK, status::StatusCode::OK,
                        status::StatusCode::OK);
    GpioLED led(gpio);

    TestRebooter rebooter;

    SystemFsm fsm(rebooter, task_scheduler, led, want_release_interval);

    TEST_ASSERT_EQUAL(status::StatusCode::Error,
                      fsm.handle_pressed(got_release_interval));
    taskYIELD();

    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());
    TEST_ASSERT_EQUAL(0, rebooter.call_count);
}

} // namespace control
} // namespace ocs
