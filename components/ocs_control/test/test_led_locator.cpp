/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <string>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "unity.h"

#include "ocs_control/gpio_led.h"
#include "ocs_control/led_locator.h"
#include "ocs_core/time.h"
#include "ocs_scheduler/async_func_scheduler.h"
#include "ocs_scheduler/constant_delay_estimator.h"
#include "ocs_scheduler/periodic_task_scheduler.h"
#include "ocs_test/test_clock.h"
#include "ocs_test/test_gpio.h"

namespace ocs {
namespace control {

namespace {

class AsyncTestRunner : private core::NonCopyable<> {
public:
    AsyncTestRunner(scheduler::ITaskScheduler& task_scheduler,
                    const char* id,
                    unsigned stack_size)
        : id_(id)
        , stack_size_(stack_size)
        , task_scheduler_(task_scheduler) {
    }

    status::StatusCode start() {
        const auto code = task_scheduler_.start();
        if (code != status::StatusCode::OK) {
            return code;
        }

        const auto ret = xTaskCreate(start_, id_.c_str(), stack_size_, this,
                                     tskIDLE_PRIORITY + 1, &handle_);

        return ret == pdTRUE ? status::StatusCode::OK : status::StatusCode::Error;
    }

    ~AsyncTestRunner() {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler_.stop());

        if (handle_) {
            vTaskDelete(handle_);
        }
    }

private:
    static void start_(void* arg) {
        TEST_ASSERT_NOT_NULL(arg);

        AsyncTestRunner& self = *static_cast<AsyncTestRunner*>(arg);
        self.run_();
    }

    void run_() {
        while (true) {
            configASSERT(task_scheduler_.run() == status::StatusCode::OK);
        }
    }

    const std::string id_;
    const unsigned stack_size_ { 0 };

    scheduler::ITaskScheduler& task_scheduler_;

    TaskHandle_t handle_ { nullptr };
};

} // namespace

TEST_CASE("LED locator: turn-on/turn-off/flip", "[ocs_control], [led_locator]") {
    test::TestClock clock;
    scheduler::ConstantDelayEstimator estimator(pdMS_TO_TICKS(10));

    scheduler::PeriodicTaskScheduler task_scheduler(clock, estimator, "test", 10);
    scheduler::AsyncFuncScheduler func_scheduler(10);

    TEST_ASSERT_EQUAL(
        status::StatusCode::OK,
        task_scheduler.add(func_scheduler, "foo", core::Duration::millisecond * 10));

    LEDLocator locator(task_scheduler, func_scheduler);

    AsyncTestRunner runner(task_scheduler, "test", 1024 * 2);
    TEST_ASSERT_EQUAL(status::StatusCode::OK, runner.start());

    test::TestGpio gpio(status::StatusCode::OK, status::StatusCode::OK,
                        status::StatusCode::OK);
    GpioLED led(gpio);

    locator.add(led);

    TEST_ASSERT_FALSE(locator.get());

    TEST_ASSERT_EQUAL(status::StatusCode::OK, locator.turn_on());
    TEST_ASSERT_TRUE(locator.get());

    auto get_flip_count = [](test::TestGpio& gpio, scheduler::AsyncFuncScheduler& ash) {
        unsigned ret = 0;

        auto future = ash.add([&gpio, &ret]() {
            ret = gpio.flip_call_count;

            return status::StatusCode::OK;
        });

        TEST_ASSERT_NOT_NULL(future);
        TEST_ASSERT_EQUAL(status::StatusCode::OK, future->wait());
        TEST_ASSERT_EQUAL(status::StatusCode::OK, future->code());

        return ret;
    };

    unsigned last_flip_count = 0;

    while (true) {
        if (get_flip_count(gpio, func_scheduler) > last_flip_count + 5) {
            break;
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }

    TEST_ASSERT_EQUAL(status::StatusCode::OK, locator.turn_off());
    TEST_ASSERT_FALSE(locator.get());

    last_flip_count = get_flip_count(gpio, func_scheduler);

    for (unsigned n = 0; n < 10; ++n) {
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

    for (unsigned n = 0; n < 10; ++n) {
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

    for (unsigned n = 0; n < 10; ++n) {
        TEST_ASSERT_EQUAL(last_flip_count, get_flip_count(gpio, func_scheduler));
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

} // namespace control
} // namespace ocs
