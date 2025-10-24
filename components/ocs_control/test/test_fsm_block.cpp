/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstring>

#include "unity.h"

#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "freertos/task.h"

#include "ocs_control/fsm_block.h"
#include "ocs_control/test/test_fsm_block_storage.h"
#include "ocs_core/noncopyable.h"
#include "ocs_test/test_clock.h"

namespace ocs {
namespace control {

namespace {

enum class State {
    None,
    First,
    Second,
    Last,
};

} // namespace

TEST_CASE("FSM block: initialization: no persistent state",
          "[ocs_control], [fsm_block]") {
    TestFsmBlockStorage storage;

    test::TestClock clock;
    const system::Time resolution = system::Duration::second;
    const char* id = "block_id";

    FsmBlock block(clock, storage, resolution, id);

    TEST_ASSERT_EQUAL(0, block.previous_state());
    TEST_ASSERT_EQUAL(0, block.current_state());
    TEST_ASSERT_EQUAL(0, block.next_state());
    TEST_ASSERT_EQUAL_INT64(0, block.previous_state_duration());
    TEST_ASSERT_EQUAL_INT64(0, block.current_state_duration());
}

TEST_CASE("FSM block: initialization: failed to read persistent state",
          "[ocs_control], [fsm_block]") {
    TestFsmBlockStorage storage(status::StatusCode::Error);
    storage.prev_state = static_cast<FsmBlock::State>(State::None);
    storage.curr_state = static_cast<FsmBlock::State>(State::First);
    storage.prev_state_duration = system::Duration::second;
    storage.curr_state_duration = system::Duration::hour;
    storage.write_count = 10;

    test::TestClock clock;

    const system::Time resolution = system::Duration::second;
    const char* id = "block_id";

    FsmBlock block(clock, storage, resolution, id);

    TEST_ASSERT_EQUAL(0, block.previous_state());
    TEST_ASSERT_EQUAL(0, block.current_state());
    TEST_ASSERT_EQUAL(0, block.next_state());
    TEST_ASSERT_EQUAL_INT64(0, block.previous_state_duration());
    TEST_ASSERT_EQUAL_INT64(0, block.current_state_duration());
}

TEST_CASE("FSM block: initialization: properly read persistent state",
          "[ocs_control], [fsm_block]") {
    TestFsmBlockStorage storage;
    storage.prev_state = static_cast<FsmBlock::State>(State::None);
    storage.curr_state = static_cast<FsmBlock::State>(State::First);
    storage.prev_state_duration = system::Duration::second;
    storage.curr_state_duration = system::Duration::hour;
    storage.write_count = 10;

    test::TestClock clock;

    const system::Time resolution = system::Duration::second;
    const char* id = "block_id";

    FsmBlock block(clock, storage, resolution, id);

    TEST_ASSERT_EQUAL(storage.prev_state, block.previous_state());
    TEST_ASSERT_EQUAL(storage.curr_state, block.current_state());
    TEST_ASSERT_EQUAL(0, block.next_state());
    TEST_ASSERT_EQUAL_INT64(storage.prev_state_duration, block.previous_state_duration());
    TEST_ASSERT_EQUAL_INT64(storage.curr_state_duration, block.current_state_duration());
}

TEST_CASE("FSM block: transit: save state", "[ocs_control], [fsm_block]") {
    const system::Time resolution = system::Duration::second;
    const char* id = "block_id";
    uint64_t write_count = 17;
    const unsigned state_duration = 42;

    TestFsmBlockStorage storage;
    storage.write_count = write_count;

    test::TestClock clock;

    FsmBlock block(clock, storage, resolution, id);

    // Reach the first state.
    block.set_next(static_cast<FsmBlock::State>(State::First));
    TEST_ASSERT_TRUE(block.is_in_transit());
    TEST_ASSERT_EQUAL(status::StatusCode::OK, block.transit());
    TEST_ASSERT_FALSE(block.is_in_transit());
    ++write_count;

    // Work for some time in the current state.
    clock.value += resolution * state_duration;
    block.update();
    TEST_ASSERT_EQUAL_INT64(state_duration, block.current_state_duration());

    // Reach the second state, so the previous state won't be none.
    block.set_next(static_cast<FsmBlock::State>(State::Second));
    TEST_ASSERT_EQUAL(status::StatusCode::OK, block.transit());
    ++write_count;

    TEST_ASSERT_EQUAL(static_cast<FsmBlock::State>(State::First), block.previous_state());
    TEST_ASSERT_EQUAL(static_cast<FsmBlock::State>(State::Second), block.current_state());
    TEST_ASSERT_EQUAL(static_cast<FsmBlock::State>(State::None), block.next_state());

    TEST_ASSERT_EQUAL_INT64(state_duration, block.previous_state_duration());
    TEST_ASSERT_EQUAL_INT64(0, block.current_state_duration());

    TEST_ASSERT_EQUAL(static_cast<FsmBlock::State>(State::First), storage.prev_state);
    TEST_ASSERT_EQUAL(static_cast<FsmBlock::State>(State::Second), storage.curr_state);
    TEST_ASSERT_EQUAL_INT64(state_duration, storage.prev_state_duration);
    TEST_ASSERT_EQUAL_INT64(0, storage.curr_state_duration);
    TEST_ASSERT_EQUAL_UINT64(write_count, storage.write_count);
}

TEST_CASE("FSM block: transit: failed to save state", "[ocs_control], [fsm_block]") {
    const system::Time resolution = system::Duration::second;
    const char* id = "block_id";
    const uint64_t write_count = 17;
    const unsigned state_duration = 42;

    TestFsmBlockStorage storage;
    storage.write_count = write_count;

    test::TestClock clock;

    FsmBlock block(clock, storage, resolution, id);

    clock.value += resolution * state_duration;
    block.update();
    TEST_ASSERT_EQUAL_INT64(state_duration, block.current_state_duration());

    block.set_next(static_cast<FsmBlock::State>(State::Second));

    const status::StatusCode write_status = status::StatusCode::Timeout;
    storage.write_status = write_status;

    TEST_ASSERT_TRUE(block.is_in_transit());
    TEST_ASSERT_EQUAL(write_status, block.transit());
    TEST_ASSERT_FALSE(block.is_in_transit());

    TEST_ASSERT_EQUAL(static_cast<FsmBlock::State>(State::None), block.previous_state());
    TEST_ASSERT_EQUAL(static_cast<FsmBlock::State>(State::Second), block.current_state());
    TEST_ASSERT_EQUAL(static_cast<FsmBlock::State>(State::None), block.next_state());
    TEST_ASSERT_EQUAL(state_duration, block.previous_state_duration());
    TEST_ASSERT_EQUAL(0, block.current_state_duration());

    TEST_ASSERT_EQUAL(0, storage.prev_state);
    TEST_ASSERT_EQUAL(0, storage.curr_state);
    TEST_ASSERT_EQUAL(0, storage.prev_state_duration);
    TEST_ASSERT_EQUAL(0, storage.curr_state_duration);
    TEST_ASSERT_EQUAL(write_count, storage.write_count);
}

TEST_CASE("FSM block: transit: reset previously saved current state duration",
          "[ocs_control], [fsm_block]") {
    const char* id = "block_id";
    const uint64_t write_count = 17;

    const system::Time resolution = system::Duration::second;
    const system::Time curr_state_duration = system::Duration::hour * 13;
    TEST_ASSERT_TRUE(curr_state_duration > resolution);

    TestFsmBlockStorage storage;
    storage.write_count = write_count;
    storage.curr_state_duration = curr_state_duration;

    test::TestClock clock;

    FsmBlock block(clock, storage, resolution, id);

    block.set_next(static_cast<FsmBlock::State>(State::Second));
    TEST_ASSERT_EQUAL(status::StatusCode::OK, block.transit());

    TEST_ASSERT_EQUAL(static_cast<FsmBlock::State>(State::None), block.previous_state());
    TEST_ASSERT_EQUAL(static_cast<FsmBlock::State>(State::Second), block.current_state());
    TEST_ASSERT_EQUAL(static_cast<FsmBlock::State>(State::None), block.next_state());
    TEST_ASSERT_EQUAL_INT64(curr_state_duration, block.previous_state_duration());
    TEST_ASSERT_EQUAL_INT64(0, block.current_state_duration());

    TEST_ASSERT_EQUAL(static_cast<FsmBlock::State>(State::None), storage.prev_state);
    TEST_ASSERT_EQUAL(static_cast<FsmBlock::State>(State::Second), storage.curr_state);
    TEST_ASSERT_EQUAL_INT64(curr_state_duration, storage.prev_state_duration);
    TEST_ASSERT_EQUAL_INT64(0, storage.curr_state_duration);
    TEST_ASSERT_EQUAL_UINT64(write_count + 1, storage.write_count);

    block.update();
    TEST_ASSERT_EQUAL_INT64(0, block.current_state_duration());
}

TEST_CASE("FSM block: save state on reboot", "[ocs_control], [fsm_block]") {
    const system::Time resolution = system::Duration::second;
    const char* id = "block_id";
    const uint64_t write_count = 17;
    const system::Time time_passed = system::Duration::second * 42;

    TestFsmBlockStorage storage;
    storage.write_count = write_count;

    test::TestClock clock;
    clock.value = time_passed;

    FsmBlock block(clock, storage, resolution, id);
    block.handle_reboot();

    TEST_ASSERT_EQUAL_UINT64(time_passed / resolution, block.current_state_duration());
    TEST_ASSERT_EQUAL_UINT64(write_count + 1, storage.write_count);
}

TEST_CASE("FSM block: save state on run", "[ocs_control], [fsm_block]") {
    const system::Time resolution = system::Duration::second;
    const char* id = "block_id";
    const uint64_t write_count = 17;

    TestFsmBlockStorage storage;
    storage.write_count = write_count;

    test::TestClock clock;

    FsmBlock block(clock, storage, resolution, id);
    TEST_ASSERT_EQUAL(status::StatusCode::OK, block.run());

    TEST_ASSERT_EQUAL_UINT64(write_count + 1, storage.write_count);
}

TEST_CASE("FSM block: update current state duration", "[ocs_control], [fsm_block]") {
    const char* id = "block_id";
    const uint64_t write_count = 17;

    const system::Time resolution = system::Duration::second;

    TestFsmBlockStorage storage;
    storage.write_count = write_count;

    test::TestClock clock;

    FsmBlock block(clock, storage, resolution, id);

    // There is no persistent state and the clock time isn't changed.
    block.update();
    TEST_ASSERT_EQUAL(0, block.current_state_duration());

    // Required resolution isn't reached.
    clock.value += resolution / 2;
    block.update();
    TEST_ASSERT_EQUAL(0, block.current_state_duration());

    // Required resolution is still not reached.
    clock.value += (resolution / 2) - system::Duration::millisecond;
    block.update();
    TEST_ASSERT_EQUAL(0, block.current_state_duration());

    // Reach the required resolution.
    clock.value += system::Duration::millisecond;
    block.update();
    TEST_ASSERT_EQUAL(1, block.current_state_duration());
}

} // namespace control
} // namespace ocs
