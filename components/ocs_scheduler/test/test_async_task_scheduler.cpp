/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <memory>
#include <vector>

#include "unity.h"

#include "ocs_scheduler/async_func_scheduler.h"
#include "ocs_scheduler/async_task.h"
#include "ocs_scheduler/async_task_scheduler.h"
#include "ocs_scheduler/constant_delay_estimator.h"
#include "ocs_system/freertos_timer_builder.h"
#include "ocs_system/heap_arena.h"
#include "ocs_test/task_scheduler_runner.h"
#include "ocs_test/test_task.h"

namespace ocs {
namespace scheduler {

namespace {

template <typename T> void run_and_wait(ITaskScheduler& scheduler, T& t) {
    while (true) {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, scheduler.run());

        if (t.wait(pdMS_TO_TICKS(30)) == status::StatusCode::OK) {
            break;
        }
    }
}

void wait_task(test::TestTask& task) {
    while (true) {
        if (task.wait(pdMS_TO_TICKS(30)) == status::StatusCode::OK) {
            break;
        }
    }
}

system::HeapArena heap_arena;
system::FreeRtosTimerBuilder freertos_timer_builder(heap_arena);

} // namespace

TEST_CASE("Async task scheduler: wait for events",
          "[async_task_scheduler], [ocs_scheduler]") {
    const char* scheduler_id = "test";
    ConstantDelayEstimator estimator(portMAX_DELAY);
    AsyncTaskScheduler scheduler(heap_arena, freertos_timer_builder, estimator,
                                 scheduler_id);

    test::TestTask task(status::StatusCode::OK);

    TEST_ASSERT_EQUAL(
        status::StatusCode::OK,
        scheduler.add(task, "test_task", system::Duration::millisecond * 50));

    TEST_ASSERT_EQUAL(status::StatusCode::OK, scheduler.start());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, scheduler.run());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, scheduler.stop());

    TEST_ASSERT_EQUAL(2, task.run_call_count());
}

TEST_CASE("Async task scheduler: register same task multiple times",
          "[async_task_scheduler], [ocs_scheduler]") {
    const char* scheduler_id = "test";
    ConstantDelayEstimator estimator(portMAX_DELAY);
    AsyncTaskScheduler scheduler(heap_arena, freertos_timer_builder, estimator,
                                 scheduler_id);

    test::TestTask task(status::StatusCode::OK);

    TEST_ASSERT_EQUAL(
        status::StatusCode::OK,
        scheduler.add(task, "test_task", system::Duration::millisecond * 50));
    TEST_ASSERT_EQUAL(
        status::StatusCode::InvalidArg,
        scheduler.add(task, "test_task", system::Duration::millisecond * 50));
}

TEST_CASE("Async task scheduler: register maximum tasks",
          "[async_task_scheduler], [ocs_scheduler]") {
    const char* scheduler_id = "test";
    ConstantDelayEstimator estimator(pdMS_TO_TICKS(30));
    AsyncTaskScheduler scheduler(heap_arena, freertos_timer_builder, estimator,
                                 scheduler_id);

    using TaskPtr = std::shared_ptr<test::TestTask>;
    std::vector<TaskPtr> tasks;

    for (size_t n = 0; n < scheduler.max_count(); ++n) {
        TaskPtr task(new (std::nothrow) test::TestTask(status::StatusCode::OK));
        TEST_ASSERT_NOT_NULL(task);

        tasks.push_back(task);
    }

    for (size_t n = 0; n < tasks.size(); ++n) {
        const std::string task_id = std::string("test_task_") + std::to_string(n);

        TEST_ASSERT_EQUAL(status::StatusCode::OK,
                          scheduler.add(*tasks[n], task_id.c_str(),
                                        system::Duration::millisecond * 30));
    }

    TEST_ASSERT_EQUAL(status::StatusCode::OK, scheduler.start());

    // Reset tasks as they are run at startup.
    for (auto& task : tasks) {
        task->reset(status::StatusCode::OK);
    }

    for (auto& task : tasks) {
        run_and_wait(scheduler, *task);
    }

    TEST_ASSERT_EQUAL(status::StatusCode::OK, scheduler.stop());
}

TEST_CASE("Async task scheduler: register maximum tasks: some failed",
          "[async_task_scheduler], [ocs_scheduler]") {
    const char* scheduler_id = "test";
    ConstantDelayEstimator estimator(pdMS_TO_TICKS(30));
    AsyncTaskScheduler scheduler(heap_arena, freertos_timer_builder, estimator,
                                 scheduler_id);

    using TaskPtr = std::shared_ptr<test::TestTask>;
    std::vector<TaskPtr> tasks;

    for (size_t n = 0; n < scheduler.max_count(); ++n) {
        status::StatusCode code = status::StatusCode::OK;
        if (n % 2 == 0) {
            code = status::StatusCode::Error;
        }

        TaskPtr task(new (std::nothrow) test::TestTask(code));
        TEST_ASSERT_NOT_NULL(task);

        tasks.push_back(task);
    }

    for (size_t n = 0; n < tasks.size(); ++n) {
        const std::string task_id = std::string("test_task_") + std::to_string(n);

        TEST_ASSERT_EQUAL(status::StatusCode::OK,
                          scheduler.add(*tasks[n], task_id.c_str(),
                                        system::Duration::millisecond * 30));
    }

    TEST_ASSERT_EQUAL(status::StatusCode::OK, scheduler.start());

    // Reset tasks as they are run at startup.
    for (auto& task : tasks) {
        task->reset(status::StatusCode::OK);
    }

    for (auto& task : tasks) {
        run_and_wait(scheduler, *task);
    }

    TEST_ASSERT_EQUAL(status::StatusCode::OK, scheduler.stop());
}

TEST_CASE("Async task scheduler: register tasks overflow",
          "[async_task_scheduler], [ocs_scheduler]") {
    const char* scheduler_id = "test";
    ConstantDelayEstimator estimator(pdMS_TO_TICKS(30));
    AsyncTaskScheduler scheduler(heap_arena, freertos_timer_builder, estimator,
                                 scheduler_id);

    using TaskPtr = std::shared_ptr<test::TestTask>;
    std::vector<TaskPtr> tasks;

    for (size_t n = 0; n < scheduler.max_count(); ++n) {
        TaskPtr task(new (std::nothrow) test::TestTask(status::StatusCode::OK));
        TEST_ASSERT_NOT_NULL(task);

        tasks.push_back(task);
    }

    for (size_t n = 0; n < tasks.size(); ++n) {
        const std::string task_id = std::string("test_task_") + std::to_string(n);

        TEST_ASSERT_EQUAL(status::StatusCode::OK,
                          scheduler.add(*tasks[n], task_id.c_str(),
                                        system::Duration::millisecond * 30));
    }

    test::TestTask task(status::StatusCode::OK);

    TEST_ASSERT_EQUAL(
        status::StatusCode::Error,
        scheduler.add(task, "test_task", system::Duration::millisecond * 10));
}

TEST_CASE("Async task scheduler: attach task",
          "[async_task_scheduler], [ocs_scheduler]") {
    const char* scheduler_id = "test";
    ConstantDelayEstimator estimator(portMAX_DELAY);
    AsyncTaskScheduler scheduler(heap_arena, freertos_timer_builder, estimator,
                                 scheduler_id);

    test::TestTask task(status::StatusCode::OK);

    EventBits_t event = 0;
    TEST_ASSERT_EQUAL(status::StatusCode::OK, scheduler.attach(event, task, "test_task"));

    test::TaskSchedulerRunner runner(scheduler, "test", 1024 * 3, tskIDLE_PRIORITY + 1);
    TEST_ASSERT_EQUAL(status::StatusCode::OK, runner.start());

    TEST_ASSERT_EQUAL(0, task.run_call_count());

    AsyncTask async_task(scheduler.get_event_group(), event);
    TEST_ASSERT_EQUAL(status::StatusCode::OK, async_task.run());

    wait_task(task);
}

TEST_CASE("Async task scheduler: add and attach task",
          "[async_task_scheduler], [ocs_scheduler]") {
    const char* scheduler_id = "test";
    ConstantDelayEstimator estimator(portMAX_DELAY);
    AsyncTaskScheduler scheduler(heap_arena, freertos_timer_builder, estimator,
                                 scheduler_id);

    test::TestTask add_task(status::StatusCode::OK);
    test::TestTask attach_task(status::StatusCode::OK);

    TEST_ASSERT_EQUAL(
        status::StatusCode::OK,
        scheduler.add(add_task, "add_task", system::Duration::millisecond * 50));

    EventBits_t event = 0;
    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      scheduler.attach(event, attach_task, "attach_task"));

    test::TaskSchedulerRunner runner(scheduler, "test", 1024 * 3, tskIDLE_PRIORITY + 1);
    TEST_ASSERT_EQUAL(status::StatusCode::OK, runner.start());

    AsyncTask async_task(scheduler.get_event_group(), event);
    TEST_ASSERT_EQUAL(status::StatusCode::OK, async_task.run());

    wait_task(add_task);
    wait_task(attach_task);
}

TEST_CASE("Async task scheduler: pause/resume task during run",
          "[async_task_scheduler], [ocs_scheduler]") {
    const char* scheduler_id = "test";

    ConstantDelayEstimator estimator(pdMS_TO_TICKS(10));

    AsyncTaskScheduler task_scheduler(heap_arena, freertos_timer_builder, estimator,
                                      scheduler_id);

    AsyncFuncScheduler func_scheduler(heap_arena, 10);

    test::TestTask task(status::StatusCode::OK);

    TEST_ASSERT_EQUAL(
        status::StatusCode::OK,
        task_scheduler.add(func_scheduler, "func", system::Duration::millisecond * 50));

    TEST_ASSERT_EQUAL(
        status::StatusCode::OK,
        task_scheduler.add(task, "task", system::Duration::millisecond * 10));

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.start());
    run_and_wait(task_scheduler, task);

    size_t task_call_count = 0;

    auto future = func_scheduler.add([&task_scheduler, &task, &task_call_count]() {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.pause(task));

        task_call_count = task.run_call_count();

        return status::StatusCode::OK;
    });
    TEST_ASSERT_NOT_NULL(future);

    run_and_wait(task_scheduler, *future);

    future = func_scheduler.add([&task, task_call_count]() {
        TEST_ASSERT_EQUAL(task_call_count, task.run_call_count());

        return status::StatusCode::OK;
    });
    TEST_ASSERT_NOT_NULL(future);

    run_and_wait(task_scheduler, *future);

    future = func_scheduler.add([&task_scheduler, &task, task_call_count]() {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.resume(task));

        TEST_ASSERT_NOT_EQUAL(task_call_count, task.run_call_count());

        return status::StatusCode::OK;
    });
    TEST_ASSERT_NOT_NULL(future);

    run_and_wait(task_scheduler, *future);
}

TEST_CASE("Async task scheduler: pause/resume task on start up",
          "[async_task_scheduler], [ocs_scheduler]") {
    const char* scheduler_id = "test";

    ConstantDelayEstimator estimator(pdMS_TO_TICKS(10));

    AsyncTaskScheduler task_scheduler(heap_arena, freertos_timer_builder, estimator,
                                      scheduler_id);

    AsyncFuncScheduler func_scheduler(heap_arena, 10);

    test::TestTask task(status::StatusCode::OK);

    TEST_ASSERT_EQUAL(
        status::StatusCode::OK,
        task_scheduler.add(func_scheduler, "func", system::Duration::millisecond * 50));

    TEST_ASSERT_EQUAL(
        status::StatusCode::OK,
        task_scheduler.add(task, "task", system::Duration::millisecond * 10));

    // Multiple pauses have no effect.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.pause(task));
    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.pause(task));
    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.pause(task));

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.start());

    auto future = func_scheduler.add([&task]() {
        TEST_ASSERT_EQUAL(0, task.run_call_count());

        return status::StatusCode::OK;
    });
    TEST_ASSERT_NOT_NULL(future);
    run_and_wait(task_scheduler, *future);

    future = func_scheduler.add([&task_scheduler, &task]() {
        // Multiple resumes have no effect.
        TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.resume(task));
        TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.resume(task));
        TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.resume(task));
        TEST_ASSERT_EQUAL(1, task.run_call_count());

        return status::StatusCode::OK;
    });
    TEST_ASSERT_NOT_NULL(future);
    run_and_wait(task_scheduler, *future);

    while (true) {
        if (task.run_call_count() > 5) {
            break;
        }

        TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.run());
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

TEST_CASE("Async task scheduler: resume non-paused task",
          "[async_task_scheduler], [ocs_scheduler]") {
    const char* scheduler_id = "test";

    ConstantDelayEstimator estimator(pdMS_TO_TICKS(10));

    AsyncTaskScheduler task_scheduler(heap_arena, freertos_timer_builder, estimator,
                                      scheduler_id);

    test::TestTask task(status::StatusCode::OK);

    TEST_ASSERT_EQUAL(
        status::StatusCode::OK,
        task_scheduler.add(task, "task", system::Duration::millisecond * 10));

    TEST_ASSERT_EQUAL(status::StatusCode::OK, task_scheduler.resume(task));
    TEST_ASSERT_EQUAL(0, task.run_call_count());
}

TEST_CASE("Async task scheduler: pause/resume unknown task",
          "[async_task_scheduler], [ocs_scheduler]") {
    const char* scheduler_id = "test";

    ConstantDelayEstimator estimator(pdMS_TO_TICKS(10));

    AsyncTaskScheduler task_scheduler(heap_arena, freertos_timer_builder, estimator,
                                      scheduler_id);

    test::TestTask task(status::StatusCode::OK);

    TEST_ASSERT_EQUAL(status::StatusCode::InvalidArg, task_scheduler.pause(task));
    TEST_ASSERT_EQUAL(status::StatusCode::InvalidArg, task_scheduler.resume(task));

    TEST_ASSERT_EQUAL(0, task.run_call_count());
}

} // namespace scheduler
} // namespace ocs
