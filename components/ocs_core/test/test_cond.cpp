/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <atomic>
#include <memory>
#include <random>
#include <string>
#include <vector>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "unity.h"

#include "ocs_core/cond.h"
#include "ocs_core/lock_guard.h"
#include "ocs_core/noncopyable.h"
#include "ocs_core/static_mutex.h"

namespace ocs {
namespace core {

namespace {

// Ref: https://stackoverflow.com/a/20136256/3873244
template <typename T> T random(T range_from, T range_to) {
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<T> distr(range_from, range_to);
    return distr(generator);
}

class TestTask : private NonCopyable<> {
public:
    TestTask(ILocker& locker, Cond& cond, const char* id, size_t stack_size)
        : id_(id)
        , stack_size_(stack_size)
        , locker_(locker)
        , cond_(cond) {
    }

    status::StatusCode start() {
        const auto ret = xTaskCreate(start_, id_.c_str(), stack_size_, this,
                                     tskIDLE_PRIORITY + 1, &handle_);

        return ret == pdTRUE ? status::StatusCode::OK : status::StatusCode::Error;
    }

    size_t awake_count() const {
        return awake_count_.load(std::memory_order_acquire);
    }

    ~TestTask() {
        TEST_ASSERT_NOT_NULL(handle_);
        vTaskDelete(handle_);
    }

private:
    static void start_(void* arg) {
        TEST_ASSERT_NOT_NULL(arg);

        TestTask& self = *static_cast<TestTask*>(arg);
        self.run_();
    }

    void run_() {
        while (true) {
            LockGuard lock(locker_);

            TEST_ASSERT_EQUAL(status::StatusCode::OK, cond_.wait(portMAX_DELAY));

            awake_count_.fetch_add(1, std::memory_order_acq_rel);
        }
    }

    const std::string id_;
    const size_t stack_size_ { 0 };

    ILocker& locker_;
    Cond& cond_;

    TaskHandle_t handle_ { nullptr };
    std::atomic<size_t> awake_count_ { 0 };
};

} // namespace

TEST_CASE("Condition variable: wait multiple tasks", "[ocs_core], [cond]") {
    StaticMutex mutex;
    Cond cond(mutex);

    const size_t task_count = 3;
    const size_t stack_size = 1024 * 2;

    using TaskPtr = std::shared_ptr<TestTask>;
    std::vector<TaskPtr> tasks;

    for (size_t n = 0; n < task_count; ++n) {
        const std::string id = "task_" + std::to_string(n);

        TaskPtr task(new (std::nothrow) TestTask(mutex, cond, id.c_str(), stack_size));
        TEST_ASSERT_NOT_NULL(task);

        tasks.emplace_back(task);
    }

    for (auto& task : tasks) {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, task->start());
    }

    auto check_tasks = [](std::vector<TaskPtr>& tasks) -> status::StatusCode {
        for (auto& task : tasks) {
            if (task->awake_count() == 0) {
                return status::StatusCode::Error;
            }
        }

        return status::StatusCode::OK;
    };

    while (true) {
        TEST_ASSERT_EQUAL(status::StatusCode::OK, cond.broadcast());

        const auto code = check_tasks(tasks);
        if (code == status::StatusCode::OK) {
            break;
        }

        // Wait for all tasks to start.
        vTaskDelay(pdMS_TO_TICKS(random(50, 100)));
    }
}

} // namespace core
} // namespace ocs
