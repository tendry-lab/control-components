/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "unity.h"

#include "ocs_core/func_value_monitor.h"
#include "ocs_core/noncopyable.h"

namespace ocs {
namespace core {

namespace {

class TestValueMonitor : public IValueMonitor<int>, private NonCopyable<> {
public:
    int get() const override {
        return value;
    }

    status::StatusCode update() override {
        ++update_call_count;

        return status::StatusCode::OK;
    }

    int value { 0 };
    size_t update_call_count { 0 };
};

} // namespace

TEST_CASE("Func value monitor: get value from the underlying monitor",
          "[func_value_monitor], [ocs_core]") {
    TestValueMonitor monitor;
    size_t func_call_count = 0;

    FuncValueMonitor<int> func_monitor(monitor, [&func_call_count]() {
        ++func_call_count;

        return status::StatusCode::OK;
    });

    monitor.value = 42;
    TEST_ASSERT_EQUAL(42, func_monitor.get());

    monitor.value = -1;
    TEST_ASSERT_EQUAL(-1, func_monitor.get());

    TEST_ASSERT_EQUAL(0, func_call_count);
    TEST_ASSERT_EQUAL(0, monitor.update_call_count);
}

TEST_CASE("Func value monitor: update calls the function",
          "[func_value_monitor], [ocs_core]") {
    TestValueMonitor monitor;
    size_t func_call_count = 0;

    FuncValueMonitor<int> func_monitor(monitor, [&func_call_count]() {
        ++func_call_count;

        return status::StatusCode::OK;
    });

    TEST_ASSERT_EQUAL(status::StatusCode::OK, func_monitor.update());
    TEST_ASSERT_EQUAL(1, func_call_count);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, func_monitor.update());
    TEST_ASSERT_EQUAL(2, func_call_count);

    // The underlying monitor is only updated by the function.
    TEST_ASSERT_EQUAL(0, monitor.update_call_count);
}

TEST_CASE("Func value monitor: update propagates the function failure",
          "[func_value_monitor], [ocs_core]") {
    TestValueMonitor monitor;

    FuncValueMonitor<int> func_monitor(monitor, []() {
        return status::StatusCode::Error;
    });

    TEST_ASSERT_EQUAL(status::StatusCode::Error, func_monitor.update());
}

} // namespace core
} // namespace ocs
