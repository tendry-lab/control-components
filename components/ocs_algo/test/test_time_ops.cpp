/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "unity.h"

#include "ocs_algo/time_ops.h"
#include "ocs_system/time.h"

namespace ocs {
namespace algo {

TEST_CASE("Time ops: after: check equal", "[ocs_algo], [time_ops]") {
    const system::Time start = 237;
    const system::Time interval = 331;
    const system::Time now = start + interval;

    TEST_ASSERT_TRUE(TimeOps::after(start, now, interval));
}

TEST_CASE("Time ops: after: check less", "[ocs_algo], [time_ops]") {
    const system::Time start = 237;
    const system::Time interval = 331;
    const system::Time now = start + interval - 1;

    TEST_ASSERT_FALSE(TimeOps::after(start, now, interval));
}

TEST_CASE("Time ops: after: check greater", "[ocs_algo], [time_ops]") {
    const system::Time start = 237;
    const system::Time interval = 331;
    const system::Time now = start + interval + 1;

    TEST_ASSERT_TRUE(TimeOps::after(start, now, interval));
}

TEST_CASE("Time ops: after: check back in time", "[ocs_algo], [time_ops]") {
    { // check signed
        const system::Time start = -123;
        const system::Time interval = 321;
        const system::Time now = start + interval;

        TEST_ASSERT_TRUE(TimeOps::after(start, now, interval));
    }

    { // check unsigned
        const TickType_t start = pdMS_TO_TICKS(0) - pdMS_TO_TICKS(123);
        const TickType_t interval = pdMS_TO_TICKS(321);
        const TickType_t now = start + interval;

        TEST_ASSERT_TRUE(TimeOps::after(start, now, interval));
    }
}

TEST_CASE("Time ops: parse time: valid input", "[ocs_algo], [time_ops]") {
    { // random number
        const char* str = "1640295065";
        const auto timestamp = TimeOps::parse_time(str);
        TEST_ASSERT_TRUE(timestamp.has_value());
        TEST_ASSERT_EQUAL(timestamp.value(), 1640295065);
    }
    { // zero number
        const char* str = "0";
        const auto timestamp = TimeOps::parse_time(str);
        TEST_ASSERT_TRUE(timestamp.has_value());
        TEST_ASSERT_EQUAL(timestamp.value(), 0);
    }
}

TEST_CASE("Time ops: parse time: invalid input", "[ocs_algo], [time_ops]") {
    const char* str = "123Invalid string";
    const auto timestamp = TimeOps::parse_time(str);
    TEST_ASSERT_FALSE(timestamp.has_value());
}

TEST_CASE("Time ops: parse time: empty input", "[ocs_algo], [time_ops]") {
    const char* str = "";
    const auto timestamp = TimeOps::parse_time(str);
    TEST_ASSERT_FALSE(timestamp.has_value());
}

TEST_CASE("Time ops: parse time: overflow", "[ocs_algo], [time_ops]") {
    const char* str = "144029506500000";
    const auto timestamp = TimeOps::parse_time(str);
    TEST_ASSERT_FALSE(timestamp.has_value());
}

} // namespace algo
} // namespace ocs
