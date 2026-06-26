/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string>

#include "unity.h"

#include "ocs_control/gpio_led.h"
#include "ocs_control/led_locator.h"
#include "ocs_core/freertos.h"
#include "ocs_scheduler/async_func_scheduler.h"
#include "ocs_scheduler/constant_delay_estimator.h"
#include "ocs_scheduler/periodic_task_scheduler.h"
#include "ocs_system/heap_arena.h"
#include "ocs_system/time.h"
#include "ocs_test/task_scheduler_runner.h"
#include "ocs_test/test_clock.h"
#include "ocs_test/test_gpio.h"

namespace ocs {
namespace control {

namespace {

system::HeapArena heap_arena;

} // namespace

TEST_CASE("LED locator: turn-on/turn-off/flip", "[ocs_control], [led_locator]") {
    test::TestClock clock;
    scheduler::ConstantDelayEstimator estimator(pdMS_TO_TICKS(10));

    scheduler::PeriodicTaskScheduler task_scheduler(heap_arena, clock, estimator, "test",
                                                    10);
    scheduler::AsyncFuncScheduler func_scheduler(heap_arena, 10);

    TEST_ASSERT_EQUAL(
        status::StatusCode::OK,
        task_scheduler.add(func_scheduler, "foo", system::Duration::millisecond * 10));

    test::TestGpio gpio(status::StatusCode::OK, status::StatusCode::OK,
                        status::StatusCode::OK);
    GpioLed led(gpio);

    LedLocator locator(task_scheduler, func_scheduler, led);

    test::TaskSchedulerRunner runner(task_scheduler, "test", 1024 * 3,
                                     tskIDLE_PRIORITY + 1);
    TEST_ASSERT_EQUAL(status::StatusCode::OK, runner.start());

    TEST_ASSERT_FALSE(locator.get());

    TEST_ASSERT_EQUAL(status::StatusCode::OK, locator.turn_on());
    TEST_ASSERT_TRUE(locator.get());

    auto get_flip_count = [](test::TestGpio& gpio, scheduler::AsyncFuncScheduler& ash) {
        size_t ret = 0;

        auto future = ash.add([&gpio, &ret]() {
            ret = gpio.flip_call_count;

            return status::StatusCode::OK;
        });

        TEST_ASSERT_NOT_NULL(future);
        TEST_ASSERT_EQUAL(status::StatusCode::OK, future->wait());
        TEST_ASSERT_EQUAL(status::StatusCode::OK, future->code());

        return ret;
    };

    size_t last_flip_count = 0;

    while (true) {
        if (get_flip_count(gpio, func_scheduler) > last_flip_count + 5) {
            break;
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }

    TEST_ASSERT_EQUAL(status::StatusCode::OK, locator.turn_off());
    TEST_ASSERT_FALSE(locator.get());

    last_flip_count = get_flip_count(gpio, func_scheduler);

    for (size_t n = 0; n < 10; ++n) {
        TEST_ASSERT_EQUAL(last_flip_count, get_flip_count(gpio, func_scheduler));
        vTaskDelay(pdMS_TO_TICKS(50));
    }

    TEST_ASSERT_EQUAL(status::StatusCode::OK, locator.flip());
    TEST_ASSERT_TRUE(locator.get());

    while (true) {
        if (get_flip_count(gpio, func_scheduler) > last_flip_count + 5) {
            break;
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }

    TEST_ASSERT_EQUAL(status::StatusCode::OK, locator.flip());
    TEST_ASSERT_FALSE(locator.get());

    last_flip_count = get_flip_count(gpio, func_scheduler);

    for (size_t n = 0; n < 10; ++n) {
        TEST_ASSERT_EQUAL(last_flip_count, get_flip_count(gpio, func_scheduler));
        vTaskDelay(pdMS_TO_TICKS(50));
    }

    TEST_ASSERT_EQUAL(status::StatusCode::OK, locator.turn_on());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, locator.turn_on());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, locator.turn_on());
    TEST_ASSERT_TRUE(locator.get());

    while (true) {
        if (get_flip_count(gpio, func_scheduler) > last_flip_count + 5) {
            break;
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }

    TEST_ASSERT_EQUAL(status::StatusCode::OK, locator.turn_off());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, locator.turn_off());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, locator.turn_off());
    TEST_ASSERT_FALSE(locator.get());

    last_flip_count = get_flip_count(gpio, func_scheduler);

    for (size_t n = 0; n < 10; ++n) {
        TEST_ASSERT_EQUAL(last_flip_count, get_flip_count(gpio, func_scheduler));
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

TEST_CASE("LED locator: disable locating when the LED is locked by another component",
          "[ocs_control], [led_locator]") {
    test::TestClock clock;
    scheduler::ConstantDelayEstimator estimator(pdMS_TO_TICKS(10));

    scheduler::PeriodicTaskScheduler task_scheduler(heap_arena, clock, estimator, "test",
                                                    10);
    scheduler::AsyncFuncScheduler func_scheduler(heap_arena, 10);

    TEST_ASSERT_EQUAL(
        status::StatusCode::OK,
        task_scheduler.add(func_scheduler, "foo", system::Duration::millisecond * 10));

    test::TestGpio gpio(status::StatusCode::OK, status::StatusCode::OK,
                        status::StatusCode::OK);
    GpioLed led(gpio);

    LedLocator locator(task_scheduler, func_scheduler, led);

    test::TaskSchedulerRunner runner(task_scheduler, "test", 1024 * 3,
                                     tskIDLE_PRIORITY + 1);
    TEST_ASSERT_EQUAL(status::StatusCode::OK, runner.start());

    TEST_ASSERT_FALSE(locator.get());

    TEST_ASSERT_EQUAL(status::StatusCode::OK, locator.turn_on());
    TEST_ASSERT_TRUE(locator.get());

    auto get_flip_count = [](test::TestGpio& gpio, scheduler::AsyncFuncScheduler& ash) {
        size_t ret = 0;

        auto future = ash.add([&gpio, &ret]() {
            ret = gpio.flip_call_count;

            return status::StatusCode::OK;
        });

        TEST_ASSERT_NOT_NULL(future);
        TEST_ASSERT_EQUAL(status::StatusCode::OK, future->wait());
        TEST_ASSERT_EQUAL(status::StatusCode::OK, future->code());

        return ret;
    };

    size_t last_flip_count = 0;

    while (true) {
        if (get_flip_count(gpio, func_scheduler) > last_flip_count + 5) {
            break;
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }

    auto lock_led = [](ILed& led, ILed::Priority priority,
                       scheduler::AsyncFuncScheduler& ash) {
        auto future = ash.add([&led, priority]() {
            return led.try_lock(priority);
        });

        TEST_ASSERT_NOT_NULL(future);
        TEST_ASSERT_EQUAL(status::StatusCode::OK, future->wait());
        TEST_ASSERT_EQUAL(status::StatusCode::OK, future->code());
    };

    lock_led(led, ILed::Priority::System, func_scheduler);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, locator.turn_off());
    TEST_ASSERT_FALSE(locator.get());

    last_flip_count = get_flip_count(gpio, func_scheduler);

    for (size_t n = 0; n < 10; ++n) {
        TEST_ASSERT_EQUAL(last_flip_count, get_flip_count(gpio, func_scheduler));
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

} // namespace control
} // namespace ocs
