/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_sensor/analog_config.h"
#include "ocs_test/memory_storage.h"
#include "ocs_test/status_storage.h"

namespace ocs {
namespace sensor {

TEST_CASE("Analog config: failed to read on initialization",
          "[ocs_sensor], [analog_config]") {
    const uint16_t def_min = 1;
    const uint16_t def_max = 2;
    const char* id = "test";

    test::MemoryStorage memory_storage;
    test::StatusStorage storage(memory_storage);

    storage.probe_status = status::StatusCode::NoData;
    storage.read_status = status::StatusCode::NoData;
    storage.write_status = status::StatusCode::NoData;

    AnalogConfig config(storage, def_min, def_max, id);
    TEST_ASSERT_TRUE(config.valid());
    TEST_ASSERT_EQUAL(def_min, config.get_min());
    TEST_ASSERT_EQUAL(def_max, config.get_max());
    TEST_ASSERT_EQUAL_STRING(id, config.get_id());
}

TEST_CASE("Analog config: invalid on initialization", "[ocs_sensor], [analog_config]") {
    const uint16_t def_min = 20;
    const uint16_t def_max = 10;
    const char* id = "test";

    TEST_ASSERT_TRUE(def_min > def_max);

    test::MemoryStorage memory_storage;
    test::StatusStorage storage(memory_storage);

    AnalogConfig config(storage, def_min, def_max, id);
    TEST_ASSERT_FALSE(config.valid());
}

TEST_CASE("Analog config: configure: OK", "[ocs_sensor], [analog_config]") {
    const uint16_t def_min = 1;
    const uint16_t def_max = 2;
    const char* id = "test";

    test::MemoryStorage memory_storage;
    test::StatusStorage storage(memory_storage);

    AnalogConfig config(storage, def_min, def_max, id);
    TEST_ASSERT_TRUE(config.valid());

    const uint16_t new_min = 10;
    const uint16_t new_max = 20;

    TEST_ASSERT_EQUAL(status::StatusCode::OK, config.configure(new_min, new_max));
    TEST_ASSERT_TRUE(config.valid());
    TEST_ASSERT_EQUAL(new_min, config.get_min());
    TEST_ASSERT_EQUAL(new_max, config.get_max());

    AnalogConfig config1(storage, def_min, def_max, id);
    TEST_ASSERT_TRUE(config1.valid());
    TEST_ASSERT_EQUAL(new_min, config1.get_min());
    TEST_ASSERT_EQUAL(new_max, config1.get_max());
}

TEST_CASE("Analog config: configure: invalid input", "[ocs_sensor], [analog_config]") {
    const uint16_t def_min = 1;
    const uint16_t def_max = 2;
    const char* id = "test";

    test::MemoryStorage memory_storage;
    test::StatusStorage storage(memory_storage);

    AnalogConfig config(storage, def_min, def_max, id);
    TEST_ASSERT_TRUE(config.valid());

    TEST_ASSERT_EQUAL(status::StatusCode::InvalidArg, config.configure(10, 10));
    TEST_ASSERT_EQUAL(status::StatusCode::InvalidArg, config.configure(20, 10));
}

TEST_CASE("Analog config: configure: failed to save", "[ocs_sensor], [analog_config]") {
    const uint16_t def_min = 1;
    const uint16_t def_max = 2;
    const char* id = "test";

    test::MemoryStorage memory_storage;
    test::StatusStorage storage(memory_storage);

    storage.write_status = status::StatusCode::Error;

    AnalogConfig config(storage, def_min, def_max, id);
    TEST_ASSERT_TRUE(config.valid());

    const uint16_t new_min = 10;
    const uint16_t new_max = 20;

    TEST_ASSERT_EQUAL(status::StatusCode::Error, config.configure(new_min, new_max));
}

TEST_CASE("Analog config: failed to reset configuration",
          "[ocs_sensor], [analog_config]") {
    const uint16_t def_min = 1;
    const uint16_t def_max = 2;
    const char* id = "test";

    test::MemoryStorage memory_storage;
    test::StatusStorage storage(memory_storage);

    storage.erase_all_status = status::StatusCode::Error;

    AnalogConfig config(storage, def_min, def_max, id);
    TEST_ASSERT_TRUE(config.valid());

    const uint16_t new_min = 10;
    const uint16_t new_max = 20;
    TEST_ASSERT_NOT_EQUAL(def_min, new_min);
    TEST_ASSERT_NOT_EQUAL(def_max, new_max);
    TEST_ASSERT_EQUAL(status::StatusCode::OK, config.configure(new_min, new_max));

    TEST_ASSERT_EQUAL(status::StatusCode::Error, config.reset());
    TEST_ASSERT_EQUAL(def_min, config.get_min());
    TEST_ASSERT_EQUAL(def_max, config.get_max());
}

TEST_CASE("Analog config: reset default configuration", "[ocs_sensor], [analog_config]") {
    const uint16_t def_min = 1;
    const uint16_t def_max = 2;
    const char* id = "test";

    test::MemoryStorage memory_storage;
    test::StatusStorage storage(memory_storage);

    AnalogConfig config(storage, def_min, def_max, id);
    TEST_ASSERT_TRUE(config.valid());

    TEST_ASSERT_EQUAL(status::StatusCode::NotModified, config.reset());
    TEST_ASSERT_EQUAL(def_min, config.get_min());
    TEST_ASSERT_EQUAL(def_max, config.get_max());
}

TEST_CASE("Analog config: configure and reset", "[ocs_sensor], [analog_config]") {
    const uint16_t def_min = 1;
    const uint16_t def_max = 2;
    const char* id = "test";

    test::MemoryStorage memory_storage;
    test::StatusStorage storage(memory_storage);

    AnalogConfig config(storage, def_min, def_max, id);
    TEST_ASSERT_TRUE(config.valid());

    const uint16_t new_min = 10;
    const uint16_t new_max = 20;

    TEST_ASSERT_EQUAL(status::StatusCode::OK, config.configure(new_min, new_max));
    TEST_ASSERT_TRUE(config.valid());
    TEST_ASSERT_EQUAL(new_min, config.get_min());
    TEST_ASSERT_EQUAL(new_max, config.get_max());

    TEST_ASSERT_EQUAL(status::StatusCode::OK, config.reset());
    TEST_ASSERT_TRUE(config.valid());
    TEST_ASSERT_EQUAL(def_min, config.get_min());
    TEST_ASSERT_EQUAL(def_max, config.get_max());

    AnalogConfig config1(storage, def_min, def_max, id);
    TEST_ASSERT_TRUE(config1.valid());
    TEST_ASSERT_EQUAL(def_min, config1.get_min());
    TEST_ASSERT_EQUAL(def_max, config1.get_max());
}

} // namespace sensor
} // namespace ocs
