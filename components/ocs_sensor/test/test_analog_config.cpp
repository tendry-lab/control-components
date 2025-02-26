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
    const uint8_t def_sample_count = 32;
    const auto def_oversampling_mode = AnalogConfig::OversamplingMode::Mode_32;

    test::MemoryStorage memory_storage;
    test::StatusStorage storage(memory_storage);

    storage.probe_status = status::StatusCode::NoData;
    storage.read_status = status::StatusCode::NoData;
    storage.write_status = status::StatusCode::NoData;

    AnalogConfig config(storage, def_min, def_max, static_cast<io::adc::Bitwidth>(10),
                        def_oversampling_mode, id);
    TEST_ASSERT_TRUE(config.valid());
    TEST_ASSERT_EQUAL(def_min, config.get_min());
    TEST_ASSERT_EQUAL(def_max, config.get_max());
    TEST_ASSERT_EQUAL(def_sample_count, config.get_sample_count());
    TEST_ASSERT_EQUAL_STRING(id, config.get_id());
}

TEST_CASE("Analog config: invalid on initialization", "[ocs_sensor], [analog_config]") {
    const char* id = "test";

    test::MemoryStorage memory_storage;
    test::StatusStorage storage(memory_storage);

    { // Invalid min/max
        AnalogConfig config(storage, 10, 1, static_cast<io::adc::Bitwidth>(10),
                            AnalogConfig::OversamplingMode::Mode_1, id);
        TEST_ASSERT_FALSE(config.valid());
    }
    { // Invalid oversampling mode
        AnalogConfig config(storage, 1, 10, static_cast<io::adc::Bitwidth>(10),
                            AnalogConfig::OversamplingMode::None, id);
        TEST_ASSERT_FALSE(config.valid());
    }
}

TEST_CASE("Analog config: configure: OK", "[ocs_sensor], [analog_config]") {
    const uint16_t def_min = 1;
    const uint16_t def_max = 2;
    const char* id = "test";
    const auto def_oversampling_mode = AnalogConfig::OversamplingMode::Mode_32;

    test::MemoryStorage memory_storage;
    test::StatusStorage storage(memory_storage);

    AnalogConfig config(storage, def_min, def_max, static_cast<io::adc::Bitwidth>(10),
                        def_oversampling_mode, id);
    TEST_ASSERT_TRUE(config.valid());

    const uint16_t new_min = 10;
    const uint16_t new_max = 20;
    const uint8_t new_sample_count = 8;

    TEST_ASSERT_NOT_EQUAL(new_min, config.get_min());
    TEST_ASSERT_NOT_EQUAL(new_max, config.get_max());
    TEST_ASSERT_NOT_EQUAL(new_sample_count, config.get_sample_count());

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      config.configure(new_min, new_max, new_sample_count));
    TEST_ASSERT_TRUE(config.valid());

    TEST_ASSERT_EQUAL(new_min, config.get_min());
    TEST_ASSERT_EQUAL(new_max, config.get_max());
    TEST_ASSERT_EQUAL(new_sample_count, config.get_sample_count());

    AnalogConfig config1(storage, def_min, def_max, static_cast<io::adc::Bitwidth>(10),
                         def_oversampling_mode, id);
    TEST_ASSERT_TRUE(config1.valid());

    TEST_ASSERT_EQUAL(new_min, config1.get_min());
    TEST_ASSERT_EQUAL(new_max, config1.get_max());
    TEST_ASSERT_EQUAL(new_sample_count, config1.get_sample_count());
}

TEST_CASE("Analog config: configure: invalid input", "[ocs_sensor], [analog_config]") {
    const uint16_t def_min = 1;
    const uint16_t def_max = 2;
    const char* id = "test";
    const uint8_t def_sample_count = 32;
    const auto def_oversampling_mode = AnalogConfig::OversamplingMode::Mode_32;

    test::MemoryStorage memory_storage;
    test::StatusStorage storage(memory_storage);

    AnalogConfig config(storage, def_min, def_max, static_cast<io::adc::Bitwidth>(10),
                        def_oversampling_mode, id);
    TEST_ASSERT_TRUE(config.valid());

    // Invalid min/max
    TEST_ASSERT_EQUAL(status::StatusCode::InvalidArg,
                      config.configure(10, 10, def_sample_count));
    // Invalid min/max
    TEST_ASSERT_EQUAL(status::StatusCode::InvalidArg,
                      config.configure(20, 10, def_sample_count));
    // Invalid sample count
    TEST_ASSERT_EQUAL(status::StatusCode::InvalidArg, config.configure(0, 10, 7));
}

TEST_CASE("Analog config: configure: failed to save", "[ocs_sensor], [analog_config]") {
    const uint16_t def_min = 1;
    const uint16_t def_max = 2;
    const char* id = "test";
    const auto def_oversampling_mode = AnalogConfig::OversamplingMode::Mode_32;

    test::MemoryStorage memory_storage;
    test::StatusStorage storage(memory_storage);

    storage.write_status = status::StatusCode::Error;

    AnalogConfig config(storage, def_min, def_max, static_cast<io::adc::Bitwidth>(10),
                        def_oversampling_mode, id);
    TEST_ASSERT_TRUE(config.valid());

    const uint16_t new_min = 10;
    const uint16_t new_max = 20;
    const uint8_t new_sample_count = 8;

    TEST_ASSERT_EQUAL(status::StatusCode::Error,
                      config.configure(new_min, new_max, new_sample_count));
}

TEST_CASE("Analog config: failed to reset configuration",
          "[ocs_sensor], [analog_config]") {
    const uint16_t def_min = 1;
    const uint16_t def_max = 2;
    const char* id = "test";
    const uint8_t def_sample_count = 32;
    const auto def_oversampling_mode = AnalogConfig::OversamplingMode::Mode_32;

    test::MemoryStorage memory_storage;
    test::StatusStorage storage(memory_storage);

    storage.erase_all_status = status::StatusCode::Error;

    AnalogConfig config(storage, def_min, def_max, static_cast<io::adc::Bitwidth>(10),
                        def_oversampling_mode, id);
    TEST_ASSERT_TRUE(config.valid());

    const uint16_t new_min = 10;
    const uint16_t new_max = 20;
    const uint8_t new_sample_count = 8;

    TEST_ASSERT_NOT_EQUAL(new_min, config.get_min());
    TEST_ASSERT_NOT_EQUAL(new_max, config.get_max());
    TEST_ASSERT_NOT_EQUAL(new_sample_count, config.get_sample_count());

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      config.configure(new_min, new_max, new_sample_count));

    TEST_ASSERT_EQUAL(status::StatusCode::Error, config.reset());
    TEST_ASSERT_TRUE(config.valid());

    TEST_ASSERT_EQUAL(def_min, config.get_min());
    TEST_ASSERT_EQUAL(def_max, config.get_max());
    TEST_ASSERT_EQUAL(def_sample_count, config.get_sample_count());
}

TEST_CASE("Analog config: reset default configuration", "[ocs_sensor], [analog_config]") {
    const uint16_t def_min = 1;
    const uint16_t def_max = 2;
    const char* id = "test";
    const uint8_t def_sample_count = 32;
    const auto def_oversampling_mode = AnalogConfig::OversamplingMode::Mode_32;

    test::MemoryStorage memory_storage;
    test::StatusStorage storage(memory_storage);

    AnalogConfig config(storage, def_min, def_max, static_cast<io::adc::Bitwidth>(10),
                        def_oversampling_mode, id);
    TEST_ASSERT_TRUE(config.valid());

    TEST_ASSERT_EQUAL(status::StatusCode::NotModified, config.reset());
    TEST_ASSERT_EQUAL(def_min, config.get_min());
    TEST_ASSERT_EQUAL(def_max, config.get_max());
    TEST_ASSERT_EQUAL(def_sample_count, config.get_sample_count());
}

TEST_CASE("Analog config: configure and reset", "[ocs_sensor], [analog_config]") {
    const uint16_t def_min = 1;
    const uint16_t def_max = 2;
    const char* id = "test";
    const uint8_t def_sample_count = 32;
    const auto def_oversampling_mode = AnalogConfig::OversamplingMode::Mode_32;

    test::MemoryStorage memory_storage;
    test::StatusStorage storage(memory_storage);

    AnalogConfig config(storage, def_min, def_max, static_cast<io::adc::Bitwidth>(10),
                        def_oversampling_mode, id);
    TEST_ASSERT_TRUE(config.valid());

    const uint16_t new_min = 10;
    const uint16_t new_max = 20;
    const uint8_t new_sample_count = 8;

    TEST_ASSERT_NOT_EQUAL(new_min, config.get_min());
    TEST_ASSERT_NOT_EQUAL(new_max, config.get_max());
    TEST_ASSERT_NOT_EQUAL(new_sample_count, config.get_sample_count());

    TEST_ASSERT_EQUAL(status::StatusCode::OK,
                      config.configure(new_min, new_max, new_sample_count));

    TEST_ASSERT_TRUE(config.valid());

    TEST_ASSERT_EQUAL(new_min, config.get_min());
    TEST_ASSERT_EQUAL(new_max, config.get_max());
    TEST_ASSERT_EQUAL(new_sample_count, config.get_sample_count());

    TEST_ASSERT_EQUAL(status::StatusCode::OK, config.reset());
    TEST_ASSERT_TRUE(config.valid());

    TEST_ASSERT_EQUAL(def_min, config.get_min());
    TEST_ASSERT_EQUAL(def_max, config.get_max());
    TEST_ASSERT_EQUAL(def_sample_count, config.get_sample_count());

    AnalogConfig config1(storage, def_min, def_max, static_cast<io::adc::Bitwidth>(10),
                         def_oversampling_mode, id);
    TEST_ASSERT_TRUE(config1.valid());

    TEST_ASSERT_EQUAL(def_min, config1.get_min());
    TEST_ASSERT_EQUAL(def_max, config1.get_max());
    TEST_ASSERT_EQUAL(def_sample_count, config1.get_sample_count());
}

TEST_CASE("Analog config: get bitwidth", "[ocs_sensor], [analog_config]") {
    const uint16_t def_min = 1;
    const uint16_t def_max = 2;
    const char* id = "test";
    const auto def_oversampling_mode = AnalogConfig::OversamplingMode::Mode_32;
    const io::adc::Bitwidth def_bitwidth = static_cast<io::adc::Bitwidth>(10);

    test::MemoryStorage memory_storage;
    test::StatusStorage storage(memory_storage);

    AnalogConfig config(storage, def_min, def_max, def_bitwidth, def_oversampling_mode,
                        id);
    TEST_ASSERT_TRUE(config.valid());

    TEST_ASSERT_EQUAL(def_bitwidth, config.get_bitwidth());
}

} // namespace sensor
} // namespace ocs
