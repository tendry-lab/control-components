/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "unity.h"

#include "ocs_diagnostic/mem_persistent_counter.h"
#include "ocs_test/memory_storage.h"
#include "ocs_test/test_counter.h"

namespace ocs {
namespace diagnostic {

TEST_CASE("Memory persistent counter: erase on initialization",
          "[ocs_diagnostic], [mem_persistent_counter]") {
    const ICounter::Value counter_value = 7;

    test::TestCounter counter("foo");
    counter.value = counter_value;

    const ICounter::Value persisted_value = 42;

    test::MemoryStorage storage;
    TEST_ASSERT_EQUAL(
        status::StatusCode::OK,
        storage.write(counter.id(), &persisted_value, sizeof(persisted_value)));

    TEST_ASSERT_TRUE(persisted_value != counter_value);
    TEST_ASSERT_TRUE(storage.contains(counter.id()));

    MemPersistentCounter persistent_counter(storage, counter);

    TEST_ASSERT_FALSE(storage.contains(counter.id()));
    TEST_ASSERT_EQUAL(persisted_value + counter_value, persistent_counter.get());
}

} // namespace diagnostic
} // namespace ocs
