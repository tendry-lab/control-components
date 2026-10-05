/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "unity.h"

#include "ocs_core/static_event_group.h"
#include "ocs_scheduler/async_func_scheduler.h"
#include "ocs_scheduler/event_func_scheduler.h"
#include "ocs_system/heap_arena.h"

namespace ocs {
namespace scheduler {

namespace {

system::HeapArena heap_arena;

} // namespace

TEST_CASE("Event func scheduler: event is posted when func is scheduled",
          "[event_func_scheduler], [ocs_scheduler]") {
    core::StaticEventGroup event_group;

    const EventBits_t event = BIT(1);

    AsyncFuncScheduler async_func_scheduler(heap_arena, 1);
    EventFuncScheduler func_scheduler(async_func_scheduler, event_group.get(), event);

    TEST_ASSERT_EQUAL(0, xEventGroupGetBits(event_group.get()));

    auto future = func_scheduler.add([]() {
        return status::StatusCode::NoData;
    });
    TEST_ASSERT_NOT_NULL(future);

    TEST_ASSERT_EQUAL(event, xEventGroupGetBits(event_group.get()));

    // The func is handled by the underlying scheduler.
    TEST_ASSERT_EQUAL(status::StatusCode::OK, async_func_scheduler.run());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, future->wait());
    TEST_ASSERT_EQUAL(status::StatusCode::NoData, future->code());
}

TEST_CASE("Event func scheduler: event isn't posted when func can't be scheduled",
          "[event_func_scheduler], [ocs_scheduler]") {
    core::StaticEventGroup event_group;

    const EventBits_t event = BIT(1);

    AsyncFuncScheduler async_func_scheduler(heap_arena, 1);
    EventFuncScheduler func_scheduler(async_func_scheduler, event_group.get(), event);

    TEST_ASSERT_NOT_NULL(func_scheduler.add([]() {
        return status::StatusCode::OK;
    }));

    xEventGroupClearBits(event_group.get(), event);

    // The underlying scheduler is full.
    TEST_ASSERT_NULL(func_scheduler.add([]() {
        return status::StatusCode::OK;
    }));

    TEST_ASSERT_EQUAL(0, xEventGroupGetBits(event_group.get()));
}

} // namespace scheduler
} // namespace ocs
