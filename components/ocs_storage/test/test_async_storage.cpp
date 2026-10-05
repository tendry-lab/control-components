/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <memory>

#include "unity.h"

#include "ocs_core/noncopyable.h"
#include "ocs_scheduler/async_func_scheduler.h"
#include "ocs_storage/async_storage.h"
#include "ocs_system/heap_arena.h"
#include "ocs_test/memory_storage.h"
#include "ocs_test/status_storage.h"

namespace ocs {
namespace storage {

namespace {

const TickType_t wait = pdMS_TO_TICKS(100);

system::HeapArena heap_arena;

//! Execute each func in place, record how many funcs were scheduled.
struct TestFuncScheduler : public scheduler::IFuncScheduler, private core::NonCopyable<> {
    FuturePtr add(Func func) override {
        if (!schedule) {
            return nullptr;
        }

        ++add_call_count;

        auto future = std::make_shared<core::Future>();
        TEST_ASSERT_EQUAL(status::StatusCode::OK, future->notify(func()));

        return future;
    }

    bool schedule { true };
    size_t add_call_count { 0 };
};

} // namespace

TEST_CASE("Async storage: operations are scheduled", "[async_storage], [ocs_storage]") {
    TestFuncScheduler func_scheduler;
    test::MemoryStorage memory_storage;
    AsyncStorage storage(heap_arena, func_scheduler, memory_storage, wait);

    const uint32_t write_value = 42;
    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      storage.write("key1", &write_value, sizeof(write_value)));
    TEST_ASSERT_EQUAL(1, func_scheduler.add_call_count);
    TEST_ASSERT_TRUE(memory_storage.contains("key1"));

    size_t size = 0;
    TEST_ASSERT_EQUAL(status::StatusCode::OK, storage.probe("key1", size));
    TEST_ASSERT_EQUAL(2, func_scheduler.add_call_count);
    TEST_ASSERT_EQUAL(sizeof(write_value), size);

    uint32_t read_value = 0;
    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      storage.read("key1", &read_value, sizeof(read_value)));
    TEST_ASSERT_EQUAL(3, func_scheduler.add_call_count);
    TEST_ASSERT_EQUAL(write_value, read_value);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, storage.erase("key1"));
    TEST_ASSERT_EQUAL(4, func_scheduler.add_call_count);
    TEST_ASSERT_FALSE(memory_storage.contains("key1"));

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      storage.write("key2", &write_value, sizeof(write_value)));
    TEST_ASSERT_EQUAL(status::StatusCode::OK, storage.erase_all());
    TEST_ASSERT_EQUAL(6, func_scheduler.add_call_count);
    TEST_ASSERT_FALSE(memory_storage.contains("key2"));
}

TEST_CASE("Async storage: operation result is propagated",
          "[async_storage], [ocs_storage]") {
    TestFuncScheduler func_scheduler;
    test::MemoryStorage memory_storage;
    test::StatusStorage status_storage(memory_storage);
    AsyncStorage storage(heap_arena, func_scheduler, status_storage, wait);

    status_storage.probe_status = status::StatusCode::Error;
    status_storage.read_status = status::StatusCode::Timeout;
    status_storage.write_status = status::StatusCode::NoMem;
    status_storage.erase_status = status::StatusCode::InvalidArg;
    status_storage.erase_all_status = status::StatusCode::InvalidState;

    size_t size = 0;
    TEST_ASSERT_EQUAL(status::StatusCode::Error, storage.probe("key", size));

    uint32_t value = 0;
    TEST_ASSERT_EQUAL(status::StatusCode::Timeout,
                      storage.read("key", &value, sizeof(value)));
    TEST_ASSERT_EQUAL(status::StatusCode::NoMem,
                      storage.write("key", &value, sizeof(value)));
    TEST_ASSERT_EQUAL(status::StatusCode::InvalidArg, storage.erase("key"));
    TEST_ASSERT_EQUAL(status::StatusCode::InvalidState, storage.erase_all());
}

TEST_CASE("Async storage: missing value is reported", "[async_storage], [ocs_storage]") {
    TestFuncScheduler func_scheduler;
    test::MemoryStorage memory_storage;
    AsyncStorage storage(heap_arena, func_scheduler, memory_storage, wait);

    size_t size = 0;
    TEST_ASSERT_EQUAL(status::StatusCode::NoData, storage.probe("key", size));

    uint32_t value = 0;
    TEST_ASSERT_EQUAL(status::StatusCode::NoData,
                      storage.read("key", &value, sizeof(value)));
}

TEST_CASE("Async storage: operation can't be scheduled",
          "[async_storage], [ocs_storage]") {
    TestFuncScheduler func_scheduler;
    func_scheduler.schedule = false;

    test::MemoryStorage memory_storage;
    AsyncStorage storage(heap_arena, func_scheduler, memory_storage, wait);

    const uint32_t value = 42;
    TEST_ASSERT_EQUAL(status::StatusCode::InvalidState,
                      storage.write("key", &value, sizeof(value)));

    TEST_ASSERT_FALSE(memory_storage.contains("key"));
}

TEST_CASE("Async storage: timed out write isn't performed later",
          "[async_storage], [ocs_storage]") {
    scheduler::AsyncFuncScheduler func_scheduler(heap_arena, 1);
    test::MemoryStorage memory_storage;
    AsyncStorage storage(heap_arena, func_scheduler, memory_storage, pdMS_TO_TICKS(10));

    // The func scheduler isn't running.
    const uint32_t value = 42;
    TEST_ASSERT_EQUAL(status::StatusCode::Timeout,
                      storage.write("key", &value, sizeof(value)));

    // The operation is cancelled, since the caller doesn't wait for it anymore.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, func_scheduler.run());
    TEST_ASSERT_FALSE(memory_storage.contains("key"));
}

TEST_CASE("Async storage: timed out read doesn't touch the caller buffer",
          "[async_storage], [ocs_storage]") {
    test::MemoryStorage memory_storage;

    const uint32_t stored_value = 42;
    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      memory_storage.write("key", &stored_value, sizeof(stored_value)));

    scheduler::AsyncFuncScheduler func_scheduler(heap_arena, 1);
    AsyncStorage storage(heap_arena, func_scheduler, memory_storage, pdMS_TO_TICKS(10));

    // The func scheduler isn't running.
    uint32_t value = 0;
    TEST_ASSERT_EQUAL(status::StatusCode::Timeout,
                      storage.read("key", &value, sizeof(value)));

    TEST_ASSERT_EQUAL(status::StatusCode::OK, func_scheduler.run());
    TEST_ASSERT_EQUAL(0, value);
}

} // namespace storage
} // namespace ocs
