/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_diagnostic/acc_persistent_counter.h"
#include "ocs_test/memory_storage.h"
#include "ocs_test/test_counter.h"

namespace ocs {
namespace diagnostic {

TEST_CASE("Accumulative persistent counter: without initial value",
          "[ocs_diagnostic], [acc_persistent_counter]") {
    const ICounter::Value counter_value = 42;

    test::MemoryStorage storage;

    test::TestCounter counter("foo");
    counter.value = counter_value;

    AccPersistentCounter persistent_counter(storage, counter);
    TEST_ASSERT_EQUAL(counter_value, persistent_counter.get());
}

TEST_CASE("Accumulative persistent counter: with initial value",
          "[ocs_diagnostic], [acc_persistent_counter]") {
    const ICounter::Value counter_value = 42;
    const ICounter::Value persisted_value = 43;

    test::MemoryStorage storage;

    test::TestCounter counter("foo");
    counter.value = counter_value;

    TEST_ASSERT_EQUAL(
        status::StatusCode::OK,
        storage.write(counter.id(), &persisted_value, sizeof(persisted_value)));

    AccPersistentCounter persistent_counter(storage, counter);
    TEST_ASSERT_EQUAL(counter_value + persisted_value, persistent_counter.get());
}

TEST_CASE("Accumulative persistent counter: handle reboot: without initial value",
          "[ocs_diagnostic], [acc_persistent_counter]") {
    const ICounter::Value counter_value = 42;

    test::MemoryStorage storage;

    test::TestCounter counter("foo");
    counter.value = counter_value;

    AccPersistentCounter persistent_counter(storage, counter);
    TEST_ASSERT_EQUAL(counter_value, persistent_counter.get());

    TEST_ASSERT_FALSE(storage.contains(counter.id()));
    persistent_counter.handle_reboot();

    ICounter::Value read_value = 0;
    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      storage.read(counter.id(), &read_value, sizeof(read_value)));

    TEST_ASSERT_EQUAL(counter_value, read_value);
}

TEST_CASE("Accumulative persistent counter: handle reboot: with initial value",
          "[ocs_diagnostic], [acc_persistent_counter]") {
    const ICounter::Value counter_value = 42;
    const ICounter::Value persisted_value = 43;

    test::MemoryStorage storage;

    test::TestCounter counter("foo");
    counter.value = counter_value;

    TEST_ASSERT_EQUAL(
        status::StatusCode::OK,
        storage.write(counter.id(), &persisted_value, sizeof(persisted_value)));

    AccPersistentCounter persistent_counter(storage, counter);
    TEST_ASSERT_EQUAL(counter_value + persisted_value, persistent_counter.get());

    ICounter::Value read_value = 0;
    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      storage.read(counter.id(), &read_value, sizeof(read_value)));
    TEST_ASSERT_EQUAL(persisted_value, read_value);

    persistent_counter.handle_reboot();

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      storage.read(counter.id(), &read_value, sizeof(read_value)));
    TEST_ASSERT_EQUAL(counter_value + persisted_value, read_value);
}

TEST_CASE("Accumulative persistent counter: save value: without previous value",
          "[ocs_diagnostic], [acc_persistent_counter]") {
    const ICounter::Value current_value = 42;
    const char* id = "foo";

    test::MemoryStorage storage;

    test::TestCounter counter(id);
    counter.value = current_value;

    AccPersistentCounter persistent_counter(storage, counter);
    TEST_ASSERT_EQUAL(current_value, persistent_counter.get());

    ICounter::Value read_value = 0;
    TEST_ASSERT_EQUAL(status::StatusCode::NoData,
                      storage.read(counter.id(), &read_value, sizeof(read_value)));

    const ICounter::Value new_value = current_value * 3;
    counter.value = new_value;

    TEST_ASSERT_EQUAL(status::StatusCode::OK, persistent_counter.run());

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      storage.read(counter.id(), &read_value, sizeof(read_value)));
    TEST_ASSERT_EQUAL(new_value, read_value);

    TEST_ASSERT_EQUAL(new_value, persistent_counter.get());
}

TEST_CASE("Accumulative persistent counter: save value: with previous value",
          "[ocs_diagnostic], [acc_persistent_counter]") {
    const ICounter::Value current_value = 42;
    const ICounter::Value persisted_value = 43;

    const char* id = "foo";

    test::MemoryStorage storage;

    test::TestCounter counter(id);
    counter.value = current_value;

    TEST_ASSERT_EQUAL(
        status::StatusCode::OK,
        storage.write(counter.id(), &persisted_value, sizeof(persisted_value)));

    AccPersistentCounter persistent_counter(storage, counter);
    TEST_ASSERT_EQUAL(current_value + persisted_value, persistent_counter.get());

    ICounter::Value read_value = 0;
    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      storage.read(counter.id(), &read_value, sizeof(read_value)));

    const ICounter::Value new_value = current_value * 3;
    counter.value = new_value;

    TEST_ASSERT_EQUAL(status::StatusCode::OK, persistent_counter.run());

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      storage.read(counter.id(), &read_value, sizeof(read_value)));
    TEST_ASSERT_EQUAL(persisted_value + new_value, read_value);

    TEST_ASSERT_EQUAL(new_value + persisted_value, persistent_counter.get());
}

TEST_CASE("Accumulative persistent counter: save value on task run",
          "[ocs_diagnostic], [acc_persistent_counter]") {
    const ICounter::Value counter_value = 42;

    test::MemoryStorage storage;

    test::TestCounter counter("foo");
    counter.value = counter_value;

    AccPersistentCounter persistent_counter(storage, counter);
    TEST_ASSERT_EQUAL(counter_value, persistent_counter.get());

    ICounter::Value read_value = 0;
    TEST_ASSERT_EQUAL(status::StatusCode::NoData,
                      storage.read(counter.id(), &read_value, sizeof(read_value)));

    TEST_ASSERT_EQUAL(status::StatusCode::OK, persistent_counter.run());

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      storage.read(counter.id(), &read_value, sizeof(read_value)));
    TEST_ASSERT_EQUAL(counter_value, read_value);
}

} // namespace diagnostic
} // namespace ocs
