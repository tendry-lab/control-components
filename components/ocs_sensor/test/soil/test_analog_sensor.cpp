/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_sensor/analog_config.h"
#include "ocs_sensor/soil/analog_sensor.h"
#include "ocs_test/memory_storage.h"
#include "ocs_test/test_clock.h"

namespace ocs {
namespace sensor {
namespace soil {

namespace {

struct TestAdcReader : public io::adc::IReader, public core::NonCopyable<> {
    status::StatusCode read(int& raw) override {
        raw = value;

        return status::StatusCode::OK;
    }

    int value { 0 };
};

struct TestAdcConverter : public io::adc::IConverter, public core::NonCopyable<> {
    status::StatusCode convert(int& voltage, int raw) override {
        voltage = raw;

        return status::StatusCode::OK;
    }
};

} // namespace

TEST_CASE("Soil analog sensor: receive in range", "[ocs_sensor], [soil_analog_sensor]") {
    const uint16_t def_min = 10;
    const uint16_t def_max = 20;
    const char* id = "test";

    test::MemoryStorage config_storage;
    AnalogConfig config(config_storage, def_min, def_max,
                        static_cast<io::adc::Bitwidth>(10),
                        AnalogConfig::OversamplingMode::Mode_1, id);
    TEST_ASSERT_TRUE(config.valid());

    test::MemoryStorage fsm_block_storage;
    test::TestClock clock;
    control::FsmBlock fsm_block(clock, fsm_block_storage, core::Duration::second,
                                "test_block");

    TestAdcReader reader;
    TestAdcConverter converter;
    AnalogSensor sensor(reader, converter, fsm_block, config);

    const int raw = def_min + 1;
    TEST_ASSERT_TRUE(raw < def_max);

    reader.value = raw;

    TEST_ASSERT_EQUAL(status::StatusCode::OK, sensor.run());

    const auto data = sensor.get_data();
    TEST_ASSERT_EQUAL(raw, data.raw);
    TEST_ASSERT_EQUAL(raw, data.voltage);
}

TEST_CASE("Soil analog sensor: receive out of range",
          "[ocs_sensor], [soil_analog_sensor]") {
    /* const uint16_t def_min = 10; */
    /* const uint16_t def_max = 20; */
    /* const char* id = "test"; */

    /* test::Memoryconfig_storage config_storage; */

    /* AnalogConfig config(config_storage, def_min, def_max, id); */
    /* TEST_ASSERT_TRUE(config.valid()); */

    /* TestAdc adc; */
    /* AnalogSensor sensor(adc, config); */

    /* int raw = def_min - 1; */
    /* reader.value = raw; */
    /* TEST_ASSERT_EQUAL(status::StatusCode::OK, sensor.run()); */
    /* /1* TEST_ASSERT_EQUAL(0, sensor.get_data().lightness); *1/ */

    /* raw = def_max + 1; */
    /* reader.value = raw; */
    /* TEST_ASSERT_EQUAL(status::StatusCode::OK, sensor.run()); */
    /* TEST_ASSERT_EQUAL(100, sensor.get_data().lightness); */
}

TEST_CASE("Soil analog sensor: read config invalid",
          "[ocs_sensor], [soil_analog_sensor]") {
    /* const uint16_t def_min = 20; */
    /* const uint16_t def_max = 10; */
    /* TEST_ASSERT_TRUE(def_min > def_max); */

    /* const char* id = "test"; */

    /* test::Memoryconfig_storage config_storage; */

    /* AnalogConfig config(config_storage, def_min, def_max, id); */
    /* TEST_ASSERT_FALSE(config.valid()); */

    /* TestAdc adc; */
    /* AnalogSensor sensor(adc, config); */

    /* const int raw = (def_min + def_max) / 2; */
    /* reader.value = raw; */

    /* TEST_ASSERT_EQUAL(status::StatusCode::InvalidState, sensor.run()); */

    /* const auto data = sensor.get_data(); */
    /* TEST_ASSERT_EQUAL(0, data.raw); */
    /* TEST_ASSERT_EQUAL(0, data.voltage); */
    /* TEST_ASSERT_EQUAL(0, data.lightness); */
}

} // namespace soil
} // namespace sensor
} // namespace ocs
