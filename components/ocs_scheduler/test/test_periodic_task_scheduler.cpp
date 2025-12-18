/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <memory>
#include <string>
#include <vector>

#include "unity.h"

#include "ocs_scheduler/async_func_scheduler.h"
#include "ocs_scheduler/constant_delay_estimator.h"
#include "ocs_scheduler/periodic_task_scheduler.h"
#include "ocs_test/test_clock.h"
#include "ocs_test/test_task.h"

namespace ocs {
namespace scheduler {

TEST_CASE("Periodic task scheduler: add", "[ocs_scheduler], [periodic_task_scheduler]") {
    const system::Time interval = system::Duration::second;
    const TickType_t delay = pdMS_TO_TICKS(10);
    const char* task_id = "test_task";

    test::TestClock clock;
    clock.value = 42;

    test::TestTask task(status::StatusCode::OK);
    ConstantDelayEstimator estimator(delay);

    PeriodicTaskScheduler task_scheduler(clock, estimator, "scheduler", 16);

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      task_scheduler.add(task, task_id, interval));

    TEST_ASSERT_EQUAL(0, task.run_call_count());

    // The first run is always allowed.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    TEST_ASSERT_EQUAL(1, task.run_call_count());
    task.reset(status::StatusCode::OK);

    clock.value += (interval - 1);
    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    TEST_ASSERT_EQUAL(0, task.run_call_count());

    clock.value += 1;
    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    TEST_ASSERT_EQUAL(1, task.run_call_count());
    task.reset(status::StatusCode::OK);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    TEST_ASSERT_EQUAL(0, task.run_call_count());
}

TEST_CASE("Periodic task scheduler: add-duplicate: pending",
          "[ocs_scheduler], [periodic_task_scheduler]") {
    const TickType_t delay = pdMS_TO_TICKS(10);

    test::TestClock clock;
    test::TestTask task(status::StatusCode::OK);
    ConstantDelayEstimator estimator(delay);

    PeriodicTaskScheduler task_scheduler(clock, estimator, "scheduler", 16);

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      task_scheduler.add(task, "test_task", system::Duration::second));
    TEST_ASSERT_EQUAL(status::StatusCode::InvalidArg,
                      task_scheduler.add(task, "test_task", system::Duration::second));
}

TEST_CASE("Periodic task scheduler: add-duplicate: active",
          "[ocs_scheduler], [periodic_task_scheduler]") {
    const TickType_t delay = pdMS_TO_TICKS(10);

    test::TestClock clock;
    test::TestTask task(status::StatusCode::OK);
    ConstantDelayEstimator estimator(delay);

    PeriodicTaskScheduler task_scheduler(clock, estimator, "scheduler", 16);

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      task_scheduler.add(task, "test_task", system::Duration::second));

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());

    TEST_ASSERT_EQUAL(status::StatusCode::InvalidArg,
                      task_scheduler.add(task, "test_task", system::Duration::second));
}

TEST_CASE("Periodic task scheduler: add-remove",
          "[ocs_scheduler], [periodic_task_scheduler]") {
    const TickType_t delay = pdMS_TO_TICKS(10);

    test::TestClock clock;
    ConstantDelayEstimator estimator(delay);

    PeriodicTaskScheduler task_scheduler(clock, estimator, "scheduler", 2);

    test::TestTask task1(status::StatusCode::OK);
    test::TestTask task2(status::StatusCode::OK);

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      task_scheduler.add(task1, "test_task_1", system::Duration::second));

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    TEST_ASSERT_EQUAL(1, task1.run_call_count());
    TEST_ASSERT_EQUAL(0, task2.run_call_count());

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      task_scheduler.add(task2, "test_task_2", system::Duration::second));
    TEST_ASSERT_EQUAL(1, task1.run_call_count());
    TEST_ASSERT_EQUAL(0, task2.run_call_count());

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.remove("test_task_1"));
    TEST_ASSERT_EQUAL(1, task1.run_call_count());
    TEST_ASSERT_EQUAL(0, task2.run_call_count());

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    TEST_ASSERT_EQUAL(1, task1.run_call_count());
    TEST_ASSERT_EQUAL(1, task2.run_call_count());
}

TEST_CASE("Periodic task scheduler: add-remove-add",
          "[ocs_scheduler], [periodic_task_scheduler]") {
    const TickType_t delay = pdMS_TO_TICKS(10);

    test::TestClock clock;
    ConstantDelayEstimator estimator(delay);

    PeriodicTaskScheduler task_scheduler(clock, estimator, "scheduler", 2);

    test::TestTask task1(status::StatusCode::OK);
    test::TestTask task2(status::StatusCode::OK);

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      task_scheduler.add(task1, "test_task_1", system::Duration::second));

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    TEST_ASSERT_EQUAL(1, task1.run_call_count());
    TEST_ASSERT_EQUAL(0, task2.run_call_count());

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      task_scheduler.add(task2, "test_task_2", system::Duration::second));
    TEST_ASSERT_EQUAL(1, task1.run_call_count());
    TEST_ASSERT_EQUAL(0, task2.run_call_count());

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.remove("test_task_1"));

    TEST_ASSERT_EQUAL(
        status::StatusCode::InvalidArg,
        task_scheduler.add(task1, "test_task_1", system::Duration::hour * 100));

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());

    TEST_ASSERT_EQUAL(1, task1.run_call_count());
    TEST_ASSERT_EQUAL(1, task2.run_call_count());

    TEST_ASSERT_EQUAL(
        status::StatusCode::OK,
        task_scheduler.add(task1, "test_task_1", system::Duration::hour * 100));

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    TEST_ASSERT_EQUAL(2, task1.run_call_count());
    TEST_ASSERT_EQUAL(2, task2.run_call_count());
}

TEST_CASE("Periodic task scheduler: add-remove-remove",
          "[ocs_scheduler], [periodic_task_scheduler]") {
    const TickType_t delay = pdMS_TO_TICKS(10);

    test::TestClock clock;
    ConstantDelayEstimator estimator(delay);

    PeriodicTaskScheduler task_scheduler(clock, estimator, "scheduler", 2);

    test::TestTask task1(status::StatusCode::OK);
    test::TestTask task2(status::StatusCode::OK);

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      task_scheduler.add(task1, "test_task_1", system::Duration::second));

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    TEST_ASSERT_EQUAL(1, task1.run_call_count());
    TEST_ASSERT_EQUAL(0, task2.run_call_count());

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      task_scheduler.add(task2, "test_task_2", system::Duration::second));
    TEST_ASSERT_EQUAL(1, task1.run_call_count());
    TEST_ASSERT_EQUAL(0, task2.run_call_count());

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.remove("test_task_1"));
    TEST_ASSERT_EQUAL(status::StatusCode::InvalidArg,
                      task_scheduler.remove("test_task_1"));
    TEST_ASSERT_EQUAL(1, task1.run_call_count());
    TEST_ASSERT_EQUAL(0, task2.run_call_count());

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    TEST_ASSERT_EQUAL(1, task1.run_call_count());
    TEST_ASSERT_EQUAL(1, task2.run_call_count());
}

TEST_CASE("Periodic task scheduler: add-remove-collapse",
          "[ocs_scheduler], [periodic_task_scheduler]") {
    const TickType_t delay = pdMS_TO_TICKS(10);

    test::TestClock clock;
    ConstantDelayEstimator estimator(delay);

    PeriodicTaskScheduler task_scheduler(clock, estimator, "scheduler", 2);

    test::TestTask task1(status::StatusCode::OK);
    test::TestTask task2(status::StatusCode::OK);

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      task_scheduler.add(task1, "test_task_1", system::Duration::second));

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    TEST_ASSERT_EQUAL(1, task1.run_call_count());
    TEST_ASSERT_EQUAL(0, task2.run_call_count());

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      task_scheduler.add(task2, "test_task_2", system::Duration::second));
    TEST_ASSERT_EQUAL(1, task1.run_call_count());
    TEST_ASSERT_EQUAL(0, task2.run_call_count());

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.remove("test_task_2"));
    TEST_ASSERT_EQUAL(1, task1.run_call_count());
    TEST_ASSERT_EQUAL(0, task2.run_call_count());

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    TEST_ASSERT_EQUAL(2, task1.run_call_count());
    TEST_ASSERT_EQUAL(0, task2.run_call_count());

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      task_scheduler.add(task2, "test_task_2", system::Duration::second));
    TEST_ASSERT_EQUAL(2, task1.run_call_count());
    TEST_ASSERT_EQUAL(0, task2.run_call_count());

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    TEST_ASSERT_EQUAL(3, task1.run_call_count());
    TEST_ASSERT_EQUAL(1, task2.run_call_count());
}

TEST_CASE("Periodic task scheduler: add-remove-run",
          "[ocs_scheduler], [periodic_task_scheduler]") {
    const TickType_t delay = pdMS_TO_TICKS(10);

    test::TestClock clock;
    ConstantDelayEstimator estimator(delay);

    AsyncFuncScheduler func_scheduler(10);
    PeriodicTaskScheduler task_scheduler(clock, estimator, "scheduler", 2);

    test::TestTask task(status::StatusCode::OK);

    TEST_ASSERT_EQUAL(
        status::StatusCode::OK,
        task_scheduler.add(func_scheduler, "func_task", system::Duration::second));

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      task_scheduler.add(task, "test_task", system::Duration::second));

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    TEST_ASSERT_EQUAL(1, task.run_call_count());

    auto future = func_scheduler.add([&task_scheduler]() {
        return task_scheduler.remove("test_task");
    });
    TEST_ASSERT_NOT_NULL(future);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    TEST_ASSERT_EQUAL(1, task.run_call_count());

    TEST_ASSERT_EQUAL(status::StatusCode::OK, future->wait());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, future->code());
}

TEST_CASE("Periodic task scheduler: add multiple tasks",
          "[ocs_scheduler], [periodic_task_scheduler]") {
    const unsigned task_count = 10;
    const TickType_t delay = pdMS_TO_TICKS(10);
    const system::Time interval = system::Duration::second;

    test::TestClock clock;
    clock.value = 42;

    ConstantDelayEstimator estimator(delay);

    PeriodicTaskScheduler task_scheduler(clock, estimator, "scheduler", 16);

    using TaskPtr = std::shared_ptr<test::TestTask>;
    using TaskList = std::vector<TaskPtr>;
    TaskList tasks;

    for (unsigned n = 0; n < task_count; ++n) {
        TaskPtr task(new (std::nothrow) test::TestTask(status::StatusCode::OK));
        TEST_ASSERT_NOT_NULL(task);

        tasks.push_back(task);
    }

    for (unsigned n = 0; n < task_count; ++n) {
        const std::string id = "test_task_" + std::to_string(n);
        TEST_ASSERT_EQUAL(status::StatusCode::OK,
                          task_scheduler.add(*tasks[n], id.c_str(), interval));
    }

    // The first run is always allowed.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    for (const auto& task : tasks) {
        TEST_ASSERT_EQUAL(1, task->run_call_count());
    }
    for (auto& task : tasks) {
        task->reset(status::StatusCode::OK);
    }

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    for (const auto& task : tasks) {
        TEST_ASSERT_EQUAL(0, task->run_call_count());
    }

    clock.value += interval;

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    for (const auto& task : tasks) {
        TEST_ASSERT_EQUAL(1, task->run_call_count());
    }
    for (auto& task : tasks) {
        task->reset(status::StatusCode::OK);
    }

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    for (const auto& task : tasks) {
        TEST_ASSERT_EQUAL(0, task->run_call_count());
    }
}

TEST_CASE("Periodic task scheduler: max number of tasks overflow: pending",
          "[ocs_scheduler], [periodic_task_scheduler]") {
    const system::Time interval = system::Duration::second;
    const TickType_t delay = pdMS_TO_TICKS(10);

    test::TestClock clock;
    clock.value = 42;

    ConstantDelayEstimator estimator(delay);
    PeriodicTaskScheduler task_scheduler(clock, estimator, "scheduler", 1);

    test::TestTask task1(status::StatusCode::OK);
    test::TestTask task2(status::StatusCode::OK);

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      task_scheduler.add(task1, "task_1", interval));

    TEST_ASSERT_EQUAL(status::StatusCode::Error,
                      task_scheduler.add(task2, "task_2", interval));

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    TEST_ASSERT_EQUAL(1, task1.run_call_count());
    TEST_ASSERT_EQUAL(0, task2.run_call_count());

    task1.reset(status::StatusCode::OK);
    task2.reset(status::StatusCode::OK);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    TEST_ASSERT_EQUAL(0, task1.run_call_count());
    TEST_ASSERT_EQUAL(0, task2.run_call_count());
}

TEST_CASE("Periodic task scheduler: max number of tasks overflow: active",
          "[ocs_scheduler], [periodic_task_scheduler]") {
    const TickType_t delay = pdMS_TO_TICKS(10);

    test::TestClock clock;
    ConstantDelayEstimator estimator(delay);

    PeriodicTaskScheduler task_scheduler(clock, estimator, "scheduler", 2);

    test::TestTask task1(status::StatusCode::OK);
    test::TestTask task2(status::StatusCode::OK);
    test::TestTask task3(status::StatusCode::OK);

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      task_scheduler.add(task1, "test_task_1", system::Duration::second));

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      task_scheduler.add(task2, "test_task_2", system::Duration::second));

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());

    TEST_ASSERT_EQUAL(status::StatusCode::Error,
                      task_scheduler.add(task3, "test_task_3", system::Duration::second));
}

TEST_CASE("Periodic task scheduler: add/remove multiple tasks",
          "[ocs_scheduler], [periodic_task_scheduler]") {
    const unsigned task_count = 10;
    const TickType_t delay = pdMS_TO_TICKS(10);
    const system::Time interval = system::Duration::second;

    test::TestClock clock;
    clock.value = 42;

    ConstantDelayEstimator estimator(delay);

    PeriodicTaskScheduler task_scheduler(clock, estimator, "scheduler", 16);

    using TaskPtr = std::shared_ptr<test::TestTask>;
    using TaskList = std::vector<TaskPtr>;
    TaskList tasks;

    for (unsigned n = 0; n < task_count; ++n) {
        TaskPtr task(new (std::nothrow) test::TestTask(status::StatusCode::OK));
        TEST_ASSERT_NOT_NULL(task);

        tasks.push_back(task);
    }

    for (unsigned n = 0; n < task_count; ++n) {
        const std::string id = "test_task_" + std::to_string(n);
        TEST_ASSERT_EQUAL(status::StatusCode::OK,
                          task_scheduler.add(*tasks[n], id.c_str(), interval));
    }

    // The first run is always allowed.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    for (const auto& task : tasks) {
        TEST_ASSERT_EQUAL(1, task->run_call_count());
    }
    for (auto& task : tasks) {
        task->reset(status::StatusCode::OK);
    }

    for (unsigned n = task_count / 2; n < task_count; ++n) {
        const std::string id = "test_task_" + std::to_string(n);
        TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.remove(id.c_str()));
    }

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    for (const auto& task : tasks) {
        TEST_ASSERT_EQUAL(0, task->run_call_count());
    }

    for (unsigned n = task_count / 2; n < task_count; ++n) {
        const std::string id = "test_task_" + std::to_string(n);
        TEST_ASSERT_EQUAL(status::StatusCode::OK,
                          task_scheduler.add(*tasks[n], id.c_str(), interval));
    }
    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    for (unsigned n = 0; n < task_count / 2; ++n) {
        TEST_ASSERT_EQUAL(0, tasks[n]->run_call_count());
    }
    // The first run is always allowed.
    for (unsigned n = task_count / 2; n < task_count; ++n) {
        TEST_ASSERT_EQUAL(1, tasks[n]->run_call_count());
    }

    for (auto& task : tasks) {
        task->reset(status::StatusCode::OK);
    }
    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    for (const auto& task : tasks) {
        TEST_ASSERT_EQUAL(0, task->run_call_count());
    }

    clock.value += interval;

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    for (const auto& task : tasks) {
        TEST_ASSERT_EQUAL(1, task->run_call_count());
    }
    for (auto& task : tasks) {
        task->reset(status::StatusCode::OK);
    }

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    for (const auto& task : tasks) {
        TEST_ASSERT_EQUAL(0, task->run_call_count());
    }
}

TEST_CASE("Periodic task scheduler: zero delay",
          "[ocs_scheduler], [periodic_task_scheduler]") {
    const system::Time interval = system::Duration::second;
    const char* task_id = "test_task";

    test::TestClock clock;
    clock.value = 42;

    test::TestTask task(status::StatusCode::OK);
    ConstantDelayEstimator estimator(0);

    PeriodicTaskScheduler task_scheduler(clock, estimator, "scheduler", 16);

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      task_scheduler.add(task, task_id, interval));

    TEST_ASSERT_EQUAL(0, task.run_call_count());

    // The first run is always allowed.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    TEST_ASSERT_EQUAL(1, task.run_call_count());
    task.reset(status::StatusCode::OK);

    clock.value += (interval - 1);
    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    TEST_ASSERT_EQUAL(0, task.run_call_count());

    clock.value += 1;
    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    TEST_ASSERT_EQUAL(1, task.run_call_count());
    task.reset(status::StatusCode::OK);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
    TEST_ASSERT_EQUAL(0, task.run_call_count());
}

} // namespace scheduler
} // namespace ocs
