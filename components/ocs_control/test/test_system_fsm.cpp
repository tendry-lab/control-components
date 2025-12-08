/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
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
    size_t call_count { 0 };

    void reboot() override {
        ++call_count;
    }
};

struct TestButton : public IButton, private core::NonCopyable<> {
    bool pressed { false };

    bool get() override {
        return pressed;
    }
};

struct TestFsrHandler : public IFsrHandler, private core::NonCopyable<> {
    size_t call_count { 0 };

    status::StatusCode handle_fsr() override {
        ++call_count;

        return status::StatusCode::OK;
    }
};

struct TestInitHandler : public scheduler::IEventHandler, private core::NonCopyable<> {
    size_t call_count { 0 };

    status::StatusCode handle_event() override {
        ++call_count;

        return status::StatusCode::OK;
    }
};

} // namespace

TEST_CASE("System FSM: button is pressed before LED reaction on system initialization",
          "[system_fsm], [ocs_control]") {
    test::TestClock clock;
    scheduler::ConstantDelayEstimator estimator(pdMS_TO_TICKS(10));
    scheduler::PeriodicTaskScheduler task_scheduler(clock, estimator, "scheduler", 16);

    test::TestGpio gpio(status::StatusCode::OK, status::StatusCode::OK,
                        status::StatusCode::OK);
    GpioLed led(gpio);

    TestRebooter rebooter;
    TestFsrHandler fsr_handler;
    TestInitHandler init_handler;
    TestButton button;

    SystemFsm fsm(rebooter, clock, task_scheduler, init_handler, fsr_handler, led, button,
                  SystemFsm::Params {});

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      fsm.handle_pressed(system::Duration::second));
    vTaskDelay(pdMS_TO_TICKS(10));

    // Start the LED reaction on system initialization.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());

    // Wait for the LED reaction on system initialization.
    while (gpio.flip_call_count != 6) {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    }
    gpio.flip_call_count = 0;

    // Button event should be ignored.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());

    // Ensure there is no LED reaction on button event.
    for (size_t n = 0; n < 10; ++n) {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
        TEST_ASSERT_EQUAL(0, gpio.flip_call_count);
    }

    TEST_ASSERT_EQUAL(0, rebooter.call_count);
    TEST_ASSERT_EQUAL(1, init_handler.call_count);
}

TEST_CASE("System FSM: button is pressed after LED reaction on system initialization",
          "[system_fsm], [ocs_control]") {
    test::TestClock clock;
    scheduler::ConstantDelayEstimator estimator(pdMS_TO_TICKS(10));
    scheduler::PeriodicTaskScheduler task_scheduler(clock, estimator, "scheduler", 16);

    test::TestGpio gpio(status::StatusCode::OK, status::StatusCode::OK,
                        status::StatusCode::OK);
    GpioLed led(gpio);

    TestRebooter rebooter;
    TestFsrHandler fsr_handler;
    TestInitHandler init_handler;
    TestButton button;

    SystemFsm fsm(rebooter, clock, task_scheduler, init_handler, fsr_handler, led, button,
                  SystemFsm::Params {});

    // Start LED reaction on system initialization.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());

    while (gpio.flip_call_count != 6) {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    }
    gpio.flip_call_count = 0;

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      fsm.handle_pressed(system::Duration::second));
    vTaskDelay(pdMS_TO_TICKS(10));

    // Handle button event.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());

    // Wait for button LED indication.
    while (gpio.flip_call_count != 2) {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    }
    gpio.flip_call_count = 0;

    TEST_ASSERT_EQUAL(1, rebooter.call_count);
    TEST_ASSERT_EQUAL(1, init_handler.call_count);
}

TEST_CASE("System FSM: button is pressed during LED reaction on system initialization",
          "[system_fsm], [ocs_control]") {
    test::TestClock clock;
    scheduler::ConstantDelayEstimator estimator(pdMS_TO_TICKS(10));
    scheduler::PeriodicTaskScheduler task_scheduler(clock, estimator, "scheduler", 16);

    test::TestGpio gpio(status::StatusCode::OK, status::StatusCode::OK,
                        status::StatusCode::OK);
    GpioLed led(gpio);

    TestRebooter rebooter;
    TestFsrHandler fsr_handler;
    TestInitHandler init_handler;
    TestButton button;

    SystemFsm fsm(rebooter, clock, task_scheduler, init_handler, fsr_handler, led, button,
                  SystemFsm::Params {});

    // Start LED reaction on system initialization.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());

    while (gpio.flip_call_count != 2) {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    }

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      fsm.handle_pressed(system::Duration::second));
    vTaskDelay(pdMS_TO_TICKS(10));

    // Button event should be ignored.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());

    while (gpio.flip_call_count != 6) {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    }
    gpio.flip_call_count = 0;

    // Button event should be ignored.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());

    // Ensure there is no LED reaction on button event.
    for (size_t n = 0; n < 10; ++n) {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
        TEST_ASSERT_EQUAL(0, gpio.flip_call_count);
    }

    TEST_ASSERT_EQUAL(0, rebooter.call_count);
    TEST_ASSERT_EQUAL(1, init_handler.call_count);
}

TEST_CASE("System FSM: button isn't released within interval",
          "[system_fsm], [ocs_control]") {
    test::TestClock clock;
    scheduler::ConstantDelayEstimator estimator(pdMS_TO_TICKS(10));
    scheduler::PeriodicTaskScheduler task_scheduler(clock, estimator, "scheduler", 16);

    test::TestGpio gpio(status::StatusCode::OK, status::StatusCode::OK,
                        status::StatusCode::OK);
    GpioLed led(gpio);

    TestRebooter rebooter;
    TestFsrHandler fsr_handler;
    TestInitHandler init_handler;
    TestButton button;

    const system::Time release_interval = system::Duration::second * 5;

    SystemFsm fsm(rebooter, clock, task_scheduler, init_handler, fsr_handler, led, button,
                  SystemFsm::Params {
                      .release_interval = release_interval,
                  });

    TEST_ASSERT_EQUAL(status::StatusCode::Error,
                      fsm.handle_pressed(release_interval + 1));
    vTaskDelay(pdMS_TO_TICKS(10));

    // Start the LED reaction on system initialization.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());

    // Wait for the LED reaction on system initialization.
    while (gpio.flip_call_count != 6) {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    }
    gpio.flip_call_count = 0;

    // Button event should be ignored.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());

    // Ensure there is no LED reaction on button event.
    for (size_t n = 0; n < 10; ++n) {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
        TEST_ASSERT_EQUAL(0, gpio.flip_call_count);
    }

    TEST_ASSERT_EQUAL(0, rebooter.call_count);
    TEST_ASSERT_EQUAL(1, init_handler.call_count);
}

TEST_CASE("System FSM: handle FSR", "[system_fsm], [ocs_control]") {
    const system::Time fsr_wait_begin_interval = system::Duration::second * 10;
    const system::Time fsr_wait_confirm_interval = system::Duration::second * 10;

    test::TestClock scheduler_clock;
    scheduler::ConstantDelayEstimator estimator(pdMS_TO_TICKS(10));
    scheduler::PeriodicTaskScheduler task_scheduler(scheduler_clock, estimator,
                                                    "scheduler", 16);

    test::TestGpio gpio(status::StatusCode::OK, status::StatusCode::OK,
                        status::StatusCode::OK);
    GpioLed led(gpio);

    TestRebooter rebooter;
    TestFsrHandler fsr_handler;
    TestInitHandler init_handler;
    TestButton button;
    test::TestClock fsm_clock;

    SystemFsm fsm(rebooter, fsm_clock, task_scheduler, init_handler, fsr_handler, led,
                  button,
                  SystemFsm::Params {
                      .fsr_wait_begin_interval = fsr_wait_begin_interval,
                      .fsr_wait_confirm_interval = fsr_wait_confirm_interval,
                  });

    // Keep the button pressed, so the FSR will be started.
    button.pressed = true;
    fsm_clock.value = 42;

    // Wait for the FSR to be started.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());
    TEST_ASSERT_EQUAL(0, fsr_handler.call_count);

    fsm_clock.value += fsr_wait_begin_interval / 2;

    // Wait for the required interval.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());
    TEST_ASSERT_EQUAL(0, fsr_handler.call_count);

    fsm_clock.value += fsr_wait_begin_interval / 2;

    // FSR is started.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());
    TEST_ASSERT_EQUAL(0, fsr_handler.call_count);

    // During the FSR the LED will be blinked periodically.
    while (gpio.flip_call_count != 8) {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    }
    gpio.flip_call_count = 0;

    // Release the button.
    button.pressed = false;

    // Wait for the FSR to be confirmed.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());
    TEST_ASSERT_EQUAL(0, fsr_handler.call_count);

    // Confirm the FSR.
    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      fsm.handle_pressed(system::Duration::second));
    vTaskDelay(pdMS_TO_TICKS(10));

    // Receive confirmation.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());
    TEST_ASSERT_EQUAL(1, fsr_handler.call_count);
    // Ensure LED reaction on FSR wait confirmation is removed.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());

    // Handle the LED reaction on the FSR completion.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());
    while (gpio.flip_call_count != 2) {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    }
    gpio.flip_call_count = 0;

    TEST_ASSERT_EQUAL(1, rebooter.call_count);
    TEST_ASSERT_EQUAL(0, init_handler.call_count);
}

TEST_CASE("System FSM: FSR canceled: released too quickly",
          "[system_fsm], [ocs_control]") {
    const system::Time fsr_wait_begin_interval = system::Duration::second * 10;
    const system::Time fsr_wait_confirm_interval = system::Duration::second * 10;

    test::TestClock scheduler_clock;
    scheduler::ConstantDelayEstimator estimator(pdMS_TO_TICKS(10));
    scheduler::PeriodicTaskScheduler task_scheduler(scheduler_clock, estimator,
                                                    "scheduler", 16);

    test::TestGpio gpio(status::StatusCode::OK, status::StatusCode::OK,
                        status::StatusCode::OK);
    GpioLed led(gpio);

    TestRebooter rebooter;
    TestFsrHandler fsr_handler;
    TestInitHandler init_handler;
    TestButton button;
    test::TestClock fsm_clock;

    SystemFsm fsm(rebooter, fsm_clock, task_scheduler, init_handler, fsr_handler, led,
                  button,
                  SystemFsm::Params {
                      .fsr_wait_begin_interval = fsr_wait_begin_interval,
                      .fsr_wait_confirm_interval = fsr_wait_confirm_interval,
                  });

    // Keep the button pressed, so the FSR will be started.
    button.pressed = true;
    fsm_clock.value = 42;

    // Wait for the FSR to be started.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());
    TEST_ASSERT_EQUAL(0, fsr_handler.call_count);

    fsm_clock.value += fsr_wait_begin_interval / 2;

    // Wait for the required interval.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());
    TEST_ASSERT_EQUAL(0, fsr_handler.call_count);

    // Release button.
    button.pressed = false;

    fsm_clock.value += fsr_wait_begin_interval / 2;

    // FSR is canceled.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());
    TEST_ASSERT_EQUAL(0, fsr_handler.call_count);

    // Ensure there is no LED reaction on button event.
    for (size_t n = 0; n < 10; ++n) {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
        TEST_ASSERT_EQUAL(0, gpio.flip_call_count);
    }

    TEST_ASSERT_EQUAL(0, rebooter.call_count);
    TEST_ASSERT_EQUAL(1, init_handler.call_count);
}

TEST_CASE("System FSM: FSR canceled: isn't confirmed within timeout",
          "[system_fsm], [ocs_control]") {
    const system::Time fsr_wait_begin_interval = system::Duration::second * 10;
    const system::Time fsr_wait_confirm_interval = system::Duration::second * 10;

    test::TestClock scheduler_clock;
    scheduler::ConstantDelayEstimator estimator(pdMS_TO_TICKS(10));
    scheduler::PeriodicTaskScheduler task_scheduler(scheduler_clock, estimator,
                                                    "scheduler", 16);

    test::TestGpio gpio(status::StatusCode::OK, status::StatusCode::OK,
                        status::StatusCode::OK);
    GpioLed led(gpio);

    TestRebooter rebooter;
    TestFsrHandler fsr_handler;
    TestInitHandler init_handler;
    TestButton button;
    test::TestClock fsm_clock;

    SystemFsm fsm(rebooter, fsm_clock, task_scheduler, init_handler, fsr_handler, led,
                  button,
                  SystemFsm::Params {
                      .fsr_wait_begin_interval = fsr_wait_begin_interval,
                      .fsr_wait_confirm_interval = fsr_wait_confirm_interval,
                  });

    // Keep the button pressed, so the FSR will be started.
    button.pressed = true;
    fsm_clock.value = 42;

    // Wait for the FSR to be started.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());
    TEST_ASSERT_EQUAL(0, fsr_handler.call_count);

    fsm_clock.value += fsr_wait_begin_interval / 2;

    // Wait for the required interval.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());
    TEST_ASSERT_EQUAL(0, fsr_handler.call_count);

    fsm_clock.value += fsr_wait_begin_interval / 2;

    // FSR is started.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());
    TEST_ASSERT_EQUAL(0, fsr_handler.call_count);

    // During the FSR the LED will be blinked periodically.
    while (gpio.flip_call_count != 8) {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    }
    gpio.flip_call_count = 0;

    // Release the button.
    button.pressed = false;

    // Wait for the FSR to be confirmed.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());
    TEST_ASSERT_EQUAL(0, fsr_handler.call_count);

    fsm_clock.value += fsr_wait_confirm_interval / 2;

    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());
    TEST_ASSERT_EQUAL(0, fsr_handler.call_count);

    fsm_clock.value += fsr_wait_confirm_interval / 2;

    // FSR is canceled.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, fsm.run());
    TEST_ASSERT_EQUAL(0, fsr_handler.call_count);

    // Ensure there is no LED reaction on button event.
    for (size_t n = 0; n < 10; ++n) {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
        TEST_ASSERT_EQUAL(0, gpio.flip_call_count);
    }

    TEST_ASSERT_EQUAL(0, rebooter.call_count);
    TEST_ASSERT_EQUAL(1, init_handler.call_count);
}

} // namespace control
} // namespace ocs
