/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "unity.h"

#include "ocs_algo/math_ops.h"
#include "ocs_sensor/analog_config.h"
#include "ocs_sensor/soil/analog_sensor.h"
#include "ocs_test/memory_storage.h"
#include "ocs_test/test_clock.h"

namespace ocs {
namespace sensor {
namespace soil {

namespace {

struct TestAdcReader : public io::adc::IReader, private core::NonCopyable<> {
    status::StatusCode read(int& raw) override {
        raw = value;

        return status::StatusCode::OK;
    }

    int value { 0 };
};

struct TestAdcConverter : public io::adc::IConverter, private core::NonCopyable<> {
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
    control::FsmBlock fsm_block(clock, fsm_block_storage, system::Duration::second,
                                "test_block");

    TestAdcReader reader;
    TestAdcConverter converter;
    AnalogSensor sensor(reader, converter, fsm_block, config,
                        AnalogSensor::Params {
                            .status_progress_threshold = 0,
                        });

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
    control::FsmBlock fsm_block(clock, fsm_block_storage, system::Duration::second,
                                "test_block");

    TestAdcReader reader;
    TestAdcConverter converter;
    AnalogSensor sensor(reader, converter, fsm_block, config,
                        AnalogSensor::Params {
                            .status_progress_threshold = 0,
                        });

    int raw = def_min - 1;
    reader.value = raw;
    TEST_ASSERT_EQUAL(status::StatusCode::OK, sensor.run());

    auto data = sensor.get_data();
    TEST_ASSERT_EQUAL_DOUBLE(0, data.status_progress);
    TEST_ASSERT_EQUAL(SoilStatus::Error, data.curr_status);

    raw = def_max + 1;
    reader.value = raw;
    TEST_ASSERT_EQUAL(status::StatusCode::OK, sensor.run());

    data = sensor.get_data();
    TEST_ASSERT_EQUAL_DOUBLE(0, data.status_progress);
    TEST_ASSERT_EQUAL(SoilStatus::Error, data.curr_status);
}

TEST_CASE("Soil analog sensor: read config invalid",
          "[ocs_sensor], [soil_analog_sensor]") {
    const uint16_t def_min = 20;
    const uint16_t def_max = 10;
    TEST_ASSERT_TRUE(def_min > def_max);

    const char* id = "test";

    test::MemoryStorage config_storage;

    AnalogConfig config(config_storage, def_min, def_max,
                        static_cast<io::adc::Bitwidth>(10),
                        AnalogConfig::OversamplingMode::Mode_1, id);
    TEST_ASSERT_FALSE(config.valid());

    test::MemoryStorage fsm_block_storage;
    test::TestClock clock;
    control::FsmBlock fsm_block(clock, fsm_block_storage, system::Duration::second,
                                "test_block");

    TestAdcReader reader;
    TestAdcConverter converter;
    AnalogSensor sensor(reader, converter, fsm_block, config,
                        AnalogSensor::Params {
                            .status_progress_threshold = 0,
                        });

    const int raw = (def_min + def_max) / 2;
    reader.value = raw;

    TEST_ASSERT_EQUAL(status::StatusCode::InvalidState, sensor.run());

    const auto data = sensor.get_data();
    AnalogSensor::Data empty_data;
    TEST_ASSERT_EQUAL_MEMORY(&empty_data, &data, sizeof(empty_data));
}

TEST_CASE("Soil analog sensor: validate each status",
          "[ocs_sensor], [soil_analog_sensor]") {
    // Saturated, Wet, Depletion, Dry.
    const uint16_t def_min = 10;
    const uint16_t def_max = 26;
    const char* id = "test";

    const unsigned status_interval =
        (def_max - def_min) / AnalogSensor::get_status_count();

    const system::Time resolution = system::Duration::second;

    test::MemoryStorage config_storage;
    AnalogConfig config(config_storage, def_min, def_max,
                        static_cast<io::adc::Bitwidth>(10),
                        AnalogConfig::OversamplingMode::Mode_1, id);
    TEST_ASSERT_TRUE(config.valid());

    test::MemoryStorage fsm_block_storage;
    test::TestClock clock;
    control::FsmBlock fsm_block(clock, fsm_block_storage, resolution, "test_block");
    TestAdcReader reader;
    TestAdcConverter converter;
    AnalogSensor sensor(reader, converter, fsm_block, config,
                        AnalogSensor::Params {
                            .status_progress_threshold = 0,
                        });

    // Check initial state, no previously saved state.
    auto data = sensor.get_data();
    AnalogSensor::Data empty_data;
    TEST_ASSERT_EQUAL_MEMORY(&empty_data, &data, sizeof(empty_data));

    // Saturated.
    reader.value = def_min;
    TEST_ASSERT_EQUAL(status::StatusCode::OK, sensor.run());

    data = sensor.get_data();
    TEST_ASSERT_EQUAL(reader.value, data.raw);
    TEST_ASSERT_EQUAL(reader.value, data.voltage);
    TEST_ASSERT_EQUAL(SoilStatus::None, data.prev_status);
    TEST_ASSERT_EQUAL(0, data.prev_status_duration);
    TEST_ASSERT_EQUAL(SoilStatus::Saturated, data.curr_status);
    TEST_ASSERT_EQUAL(0, data.curr_status_duration);
    TEST_ASSERT_EQUAL_DOUBLE(0, data.status_progress);
    TEST_ASSERT_EQUAL(1, data.write_count);

    // Still saturated.
    clock.value += resolution;
    reader.value += status_interval / 2;
    TEST_ASSERT_EQUAL(status::StatusCode::OK, sensor.run());

    data = sensor.get_data();
    TEST_ASSERT_EQUAL(reader.value, data.raw);
    TEST_ASSERT_EQUAL(reader.value, data.voltage);
    TEST_ASSERT_EQUAL(SoilStatus::None, data.prev_status);
    TEST_ASSERT_EQUAL(0, data.prev_status_duration);
    TEST_ASSERT_EQUAL(SoilStatus::Saturated, data.curr_status);
    TEST_ASSERT_EQUAL(1, data.curr_status_duration);
    TEST_ASSERT_EQUAL_DOUBLE(50, data.status_progress);
    TEST_ASSERT_EQUAL(1, data.write_count);

    // Wet.
    reader.value += status_interval / 2;
    clock.value += resolution;
    TEST_ASSERT_EQUAL(status::StatusCode::OK, sensor.run());

    data = sensor.get_data();
    TEST_ASSERT_EQUAL(reader.value, data.raw);
    TEST_ASSERT_EQUAL(reader.value, data.voltage);
    TEST_ASSERT_EQUAL(SoilStatus::Saturated, data.prev_status);
    TEST_ASSERT_EQUAL(2, data.prev_status_duration);
    TEST_ASSERT_EQUAL(SoilStatus::Wet, data.curr_status);
    TEST_ASSERT_EQUAL(0, data.curr_status_duration);
    TEST_ASSERT_EQUAL_DOUBLE(0, data.status_progress);
    TEST_ASSERT_EQUAL(2, data.write_count);

    // Still wet.
    clock.value += resolution;
    reader.value += status_interval / 2;
    TEST_ASSERT_EQUAL(status::StatusCode::OK, sensor.run());

    data = sensor.get_data();
    TEST_ASSERT_EQUAL(reader.value, data.raw);
    TEST_ASSERT_EQUAL(reader.value, data.voltage);
    TEST_ASSERT_EQUAL(SoilStatus::Saturated, data.prev_status);
    TEST_ASSERT_EQUAL(2, data.prev_status_duration);
    TEST_ASSERT_EQUAL(SoilStatus::Wet, data.curr_status);
    TEST_ASSERT_EQUAL(1, data.curr_status_duration);
    TEST_ASSERT_EQUAL_DOUBLE(50, data.status_progress);
    TEST_ASSERT_EQUAL(2, data.write_count);

    // Depletion
    reader.value += status_interval / 2;
    clock.value += resolution;
    TEST_ASSERT_EQUAL(status::StatusCode::OK, sensor.run());

    data = sensor.get_data();
    TEST_ASSERT_EQUAL(reader.value, data.raw);
    TEST_ASSERT_EQUAL(reader.value, data.voltage);
    TEST_ASSERT_EQUAL(SoilStatus::Wet, data.prev_status);
    TEST_ASSERT_EQUAL(2, data.prev_status_duration);
    TEST_ASSERT_EQUAL(SoilStatus::Depletion, data.curr_status);
    TEST_ASSERT_EQUAL(0, data.curr_status_duration);
    TEST_ASSERT_EQUAL_DOUBLE(0, data.status_progress);
    TEST_ASSERT_EQUAL(3, data.write_count);

    // Still depletion.
    clock.value += resolution;
    reader.value += status_interval / 2;
    TEST_ASSERT_EQUAL(status::StatusCode::OK, sensor.run());

    data = sensor.get_data();
    TEST_ASSERT_EQUAL(reader.value, data.raw);
    TEST_ASSERT_EQUAL(reader.value, data.voltage);
    TEST_ASSERT_EQUAL(SoilStatus::Wet, data.prev_status);
    TEST_ASSERT_EQUAL(2, data.prev_status_duration);
    TEST_ASSERT_EQUAL(SoilStatus::Depletion, data.curr_status);
    TEST_ASSERT_EQUAL(1, data.curr_status_duration);
    TEST_ASSERT_EQUAL_DOUBLE(50, data.status_progress);
    TEST_ASSERT_EQUAL(3, data.write_count);

    // Dry.
    reader.value += status_interval / 2;
    clock.value += resolution;
    TEST_ASSERT_EQUAL(status::StatusCode::OK, sensor.run());

    data = sensor.get_data();
    TEST_ASSERT_EQUAL(reader.value, data.raw);
    TEST_ASSERT_EQUAL(reader.value, data.voltage);
    TEST_ASSERT_EQUAL(SoilStatus::Depletion, data.prev_status);
    TEST_ASSERT_EQUAL(2, data.prev_status_duration);
    TEST_ASSERT_EQUAL(SoilStatus::Dry, data.curr_status);
    TEST_ASSERT_EQUAL(0, data.curr_status_duration);
    TEST_ASSERT_EQUAL_DOUBLE(0, data.status_progress);
    TEST_ASSERT_EQUAL(4, data.write_count);

    // Still dry.
    clock.value += resolution;
    reader.value += status_interval / 2;
    TEST_ASSERT_EQUAL(status::StatusCode::OK, sensor.run());

    data = sensor.get_data();
    TEST_ASSERT_EQUAL(reader.value, data.raw);
    TEST_ASSERT_EQUAL(reader.value, data.voltage);
    TEST_ASSERT_EQUAL(SoilStatus::Depletion, data.prev_status);
    TEST_ASSERT_EQUAL(2, data.prev_status_duration);
    TEST_ASSERT_EQUAL(SoilStatus::Dry, data.curr_status);
    TEST_ASSERT_EQUAL(1, data.curr_status_duration);
    TEST_ASSERT_EQUAL_DOUBLE(50, data.status_progress);
    TEST_ASSERT_EQUAL(4, data.write_count);

    // Error.
    reader.value += (status_interval / 2) + 1;
    clock.value += resolution;
    TEST_ASSERT_EQUAL(status::StatusCode::OK, sensor.run());

    data = sensor.get_data();
    TEST_ASSERT_EQUAL(reader.value, data.raw);
    TEST_ASSERT_EQUAL(reader.value, data.voltage);
    TEST_ASSERT_EQUAL(SoilStatus::Dry, data.prev_status);
    TEST_ASSERT_EQUAL(2, data.prev_status_duration);
    TEST_ASSERT_EQUAL(SoilStatus::Error, data.curr_status);
    TEST_ASSERT_EQUAL(0, data.curr_status_duration);
    TEST_ASSERT_EQUAL_DOUBLE(0, data.status_progress);
    TEST_ASSERT_EQUAL(5, data.write_count);

    // Still error.
    clock.value += resolution;
    reader.value += status_interval / 2;
    TEST_ASSERT_EQUAL(status::StatusCode::OK, sensor.run());

    data = sensor.get_data();
    TEST_ASSERT_EQUAL(reader.value, data.raw);
    TEST_ASSERT_EQUAL(reader.value, data.voltage);
    TEST_ASSERT_EQUAL(SoilStatus::Dry, data.prev_status);
    TEST_ASSERT_EQUAL(2, data.prev_status_duration);
    TEST_ASSERT_EQUAL(SoilStatus::Error, data.curr_status);
    TEST_ASSERT_EQUAL(1, data.curr_status_duration);
    TEST_ASSERT_EQUAL_DOUBLE(0, data.status_progress);
    TEST_ASSERT_EQUAL(5, data.write_count);
}

TEST_CASE("Soil analog sensor: read initial status from storage",
          "[ocs_sensor], [soil_analog_sensor]") {
    const uint16_t def_min = 10;
    const uint16_t def_max = 26;
    const char* id = "test";
    const system::Time resolution = system::Duration::second;

    const unsigned status_interval =
        (def_max - def_min) / AnalogSensor::get_status_count();

    test::MemoryStorage config_storage;
    AnalogConfig config(config_storage, def_min, def_max,
                        static_cast<io::adc::Bitwidth>(10),
                        AnalogConfig::OversamplingMode::Mode_1, id);
    TEST_ASSERT_TRUE(config.valid());

    test::TestClock clock;
    test::MemoryStorage fsm_block_storage;

    TestAdcReader reader;
    TestAdcConverter converter;

    control::FsmBlock fsm_block1(clock, fsm_block_storage, resolution, "test_block1");
    AnalogSensor sensor1(reader, converter, fsm_block1, config,
                         AnalogSensor::Params {
                             .status_progress_threshold = 0,
                         });

    // Saturated.
    reader.value = def_min;
    TEST_ASSERT_EQUAL(status::StatusCode::OK, sensor1.run());

    auto data1 = sensor1.get_data();
    TEST_ASSERT_EQUAL(reader.value, data1.raw);
    TEST_ASSERT_EQUAL(reader.value, data1.voltage);
    TEST_ASSERT_EQUAL(SoilStatus::None, data1.prev_status);
    TEST_ASSERT_EQUAL(0, data1.prev_status_duration);
    TEST_ASSERT_EQUAL(SoilStatus::Saturated, data1.curr_status);
    TEST_ASSERT_EQUAL(0, data1.curr_status_duration);
    TEST_ASSERT_EQUAL_DOUBLE(0, data1.status_progress);
    TEST_ASSERT_EQUAL(1, data1.write_count);

    // Still saturated.
    clock.value += resolution;
    TEST_ASSERT_EQUAL(status::StatusCode::OK, sensor1.run());

    data1 = sensor1.get_data();
    TEST_ASSERT_EQUAL(reader.value, data1.raw);
    TEST_ASSERT_EQUAL(reader.value, data1.voltage);
    TEST_ASSERT_EQUAL(SoilStatus::None, data1.prev_status);
    TEST_ASSERT_EQUAL(0, data1.prev_status_duration);
    TEST_ASSERT_EQUAL(SoilStatus::Saturated, data1.curr_status);
    TEST_ASSERT_EQUAL(1, data1.curr_status_duration);
    TEST_ASSERT_EQUAL_DOUBLE(0, data1.status_progress);
    TEST_ASSERT_EQUAL(1, data1.write_count);

    control::FsmBlock fsm_block2(clock, fsm_block_storage, resolution, "test_block2");
    AnalogSensor sensor2(reader, converter, fsm_block2, config,
                         AnalogSensor::Params {
                             .status_progress_threshold = 0,
                         });

    // Wet.
    reader.value += status_interval;
    clock.value += resolution;
    TEST_ASSERT_EQUAL(status::StatusCode::OK, sensor2.run());

    const auto data2 = sensor2.get_data();
    TEST_ASSERT_EQUAL(reader.value, data2.raw);
    TEST_ASSERT_EQUAL(reader.value, data2.voltage);
    TEST_ASSERT_EQUAL(SoilStatus::Saturated, data2.prev_status);
    TEST_ASSERT_EQUAL(2, data2.prev_status_duration);
    TEST_ASSERT_EQUAL(SoilStatus::Wet, data2.curr_status);
    TEST_ASSERT_EQUAL(0, data2.curr_status_duration);
    TEST_ASSERT_EQUAL_DOUBLE(0, data2.status_progress);
    TEST_ASSERT_EQUAL(2, data2.write_count);
}

TEST_CASE("Soil analog sensor: ignore changes close to the threshold: valid states",
          "[ocs_sensor], [soil_analog_sensor]") {
    const uint16_t def_min = 10;
    const uint16_t def_max = 26;
    const char* id = "test";
    const system::Time resolution = system::Duration::second;

    const uint16_t range = def_max - def_min;
    TEST_ASSERT_TRUE((range % AnalogSensor::get_status_count()) == 0);

    const unsigned status_interval = range / AnalogSensor::get_status_count();
    const uint8_t status_threshold = 49;
    const double moisture_per_off = static_cast<double>(1) / range;

    test::MemoryStorage config_storage;
    AnalogConfig config(config_storage, def_min, def_max,
                        static_cast<io::adc::Bitwidth>(10),
                        AnalogConfig::OversamplingMode::Mode_1, id);
    TEST_ASSERT_TRUE(config.valid());

    test::MemoryStorage fsm_block_storage;
    test::TestClock clock;
    control::FsmBlock fsm_block(clock, fsm_block_storage, resolution, "test_block");

    TestAdcReader reader;
    TestAdcConverter converter;
    AnalogSensor sensor(reader, converter, fsm_block, config,
                        AnalogSensor::Params {
                            .status_progress_threshold = status_threshold,
                        });

    reader.value = def_min;
    double expected_moisture = 100;

    TEST_ASSERT_EQUAL(status::StatusCode::OK, sensor.run());

    auto data = sensor.get_data();

    // Just started, no previous status, threshold shouldn't be taken into account.
    TEST_ASSERT_EQUAL(reader.value, data.raw);
    TEST_ASSERT_EQUAL(reader.value, data.voltage);
    TEST_ASSERT_EQUAL_DOUBLE(expected_moisture, data.moisture);
    TEST_ASSERT_EQUAL(SoilStatus::None, data.prev_status);
    TEST_ASSERT_EQUAL(0, data.prev_status_duration);
    TEST_ASSERT_EQUAL(SoilStatus::Saturated, data.curr_status);
    TEST_ASSERT_EQUAL(0, data.curr_status_duration);
    TEST_ASSERT_EQUAL_DOUBLE(0, data.status_progress);
    TEST_ASSERT_EQUAL(1, data.write_count);

    // Still saturated.
    clock.value += resolution;
    reader.value += status_interval / 2;
    expected_moisture -=
        algo::MathOps::round_floor(100 * moisture_per_off * (status_interval / 2), 2);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, sensor.run());

    data = sensor.get_data();
    TEST_ASSERT_EQUAL(reader.value, data.raw);
    TEST_ASSERT_EQUAL(reader.value, data.voltage);
    TEST_ASSERT_EQUAL_DOUBLE(expected_moisture, data.moisture);
    TEST_ASSERT_EQUAL(SoilStatus::None, data.prev_status);
    TEST_ASSERT_EQUAL(0, data.prev_status_duration);
    TEST_ASSERT_EQUAL(SoilStatus::Saturated, data.curr_status);
    TEST_ASSERT_EQUAL(1, data.curr_status_duration);
    TEST_ASSERT_EQUAL_DOUBLE(50, data.status_progress);
    TEST_ASSERT_EQUAL(1, data.write_count);

    // Saturated has finished. Now wet, but below the required threshold, which means we
    // aren't sure that this status is stable, maybe on the next sensor reading we will
    // move back to the saturated status.
    clock.value += resolution;
    reader.value += status_interval / 2;

    expected_moisture -=
        algo::MathOps::round_floor(100 * moisture_per_off * (status_interval / 2), 2);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, sensor.run());

    data = sensor.get_data();
    TEST_ASSERT_EQUAL(reader.value, data.raw);
    TEST_ASSERT_EQUAL(reader.value, data.voltage);
    TEST_ASSERT_EQUAL_DOUBLE(expected_moisture, data.moisture);
    TEST_ASSERT_EQUAL(SoilStatus::None, data.prev_status);
    TEST_ASSERT_EQUAL(0, data.prev_status_duration);
    TEST_ASSERT_EQUAL(SoilStatus::Saturated, data.curr_status);
    TEST_ASSERT_EQUAL(2, data.curr_status_duration);
    TEST_ASSERT_EQUAL_DOUBLE(99.99, data.status_progress);
    TEST_ASSERT_EQUAL(1, data.write_count);

    clock.value += resolution;
    // Threshold hasn't been reached, still saturated.
    reader.value += (status_interval / 2) - 1;

    TEST_ASSERT_EQUAL(status::StatusCode::OK, sensor.run());

    data = sensor.get_data();
    TEST_ASSERT_EQUAL(reader.value, data.raw);
    TEST_ASSERT_EQUAL(reader.value, data.voltage);
    TEST_ASSERT_EQUAL_DOUBLE(expected_moisture, data.moisture);
    TEST_ASSERT_EQUAL(SoilStatus::None, data.prev_status);
    TEST_ASSERT_EQUAL(0, data.prev_status_duration);
    TEST_ASSERT_EQUAL(SoilStatus::Saturated, data.curr_status);
    TEST_ASSERT_EQUAL(3, data.curr_status_duration);
    TEST_ASSERT_EQUAL_DOUBLE(99.99, data.status_progress);
    TEST_ASSERT_EQUAL(1, data.write_count);

    clock.value += resolution;
    // Threshold has been reached, now wet.
    reader.value += 1;

    expected_moisture -=
        algo::MathOps::round_floor(100 * moisture_per_off * (status_interval / 2), 2);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, sensor.run());

    data = sensor.get_data();
    TEST_ASSERT_EQUAL(reader.value, data.raw);
    TEST_ASSERT_EQUAL(reader.value, data.voltage);
    TEST_ASSERT_EQUAL_DOUBLE(expected_moisture, data.moisture);
    TEST_ASSERT_EQUAL(SoilStatus::Saturated, data.prev_status);
    TEST_ASSERT_EQUAL(4, data.prev_status_duration);
    TEST_ASSERT_EQUAL(SoilStatus::Wet, data.curr_status);
    TEST_ASSERT_EQUAL(0, data.curr_status_duration);
    TEST_ASSERT_EQUAL_DOUBLE(50, data.status_progress);
    TEST_ASSERT_EQUAL(2, data.write_count);
}

TEST_CASE("Soil analog sensor: ignore changes close to the threshold: invalid states",
          "[ocs_sensor], [soil_analog_sensor]") {
    const uint16_t def_min = 10;
    const uint16_t def_max = 26;
    const char* id = "test";
    const system::Time resolution = system::Duration::second;

    const uint16_t range = def_max - def_min;
    TEST_ASSERT_TRUE((range % AnalogSensor::get_status_count()) == 0);

    const unsigned status_interval = range / AnalogSensor::get_status_count();
    const uint8_t status_threshold = 49;
    const double moisture_per_off = static_cast<double>(1) / range;

    test::MemoryStorage config_storage;
    AnalogConfig config(config_storage, def_min, def_max,
                        static_cast<io::adc::Bitwidth>(10),
                        AnalogConfig::OversamplingMode::Mode_1, id);
    TEST_ASSERT_TRUE(config.valid());

    test::MemoryStorage fsm_block_storage;
    test::TestClock clock;
    control::FsmBlock fsm_block(clock, fsm_block_storage, resolution, "test_block");

    TestAdcReader reader;
    TestAdcConverter converter;
    AnalogSensor sensor(reader, converter, fsm_block, config,
                        AnalogSensor::Params {
                            .status_progress_threshold = status_threshold,
                        });

    reader.value = def_min;
    double expected_moisture = 100;

    TEST_ASSERT_EQUAL(status::StatusCode::OK, sensor.run());

    auto data = sensor.get_data();

    // Just started, no previous status, threshold shouldn't be taken into account.
    TEST_ASSERT_EQUAL(reader.value, data.raw);
    TEST_ASSERT_EQUAL(reader.value, data.voltage);
    TEST_ASSERT_EQUAL_DOUBLE(expected_moisture, data.moisture);
    TEST_ASSERT_EQUAL(SoilStatus::None, data.prev_status);
    TEST_ASSERT_EQUAL(0, data.prev_status_duration);
    TEST_ASSERT_EQUAL(SoilStatus::Saturated, data.curr_status);
    TEST_ASSERT_EQUAL(0, data.curr_status_duration);
    TEST_ASSERT_EQUAL_DOUBLE(0, data.status_progress);
    TEST_ASSERT_EQUAL(1, data.write_count);

    // Move to error state immediately.
    clock.value += resolution;
    reader.value = def_min - 1;

    TEST_ASSERT_EQUAL(status::StatusCode::OK, sensor.run());

    data = sensor.get_data();

    TEST_ASSERT_EQUAL(reader.value, data.raw);
    TEST_ASSERT_EQUAL(reader.value, data.voltage);
    TEST_ASSERT_EQUAL_DOUBLE(0, data.moisture);
    TEST_ASSERT_EQUAL(SoilStatus::Saturated, data.prev_status);
    TEST_ASSERT_EQUAL(1, data.prev_status_duration);
    TEST_ASSERT_EQUAL(SoilStatus::Error, data.curr_status);
    TEST_ASSERT_EQUAL(0, data.curr_status_duration);
    TEST_ASSERT_EQUAL_DOUBLE(0, data.status_progress);
    TEST_ASSERT_EQUAL(2, data.write_count);

    // Try to leave error state.
    clock.value += resolution;
    // Still error, threshold hasn't been reached.
    reader.value = def_min;

    TEST_ASSERT_EQUAL(status::StatusCode::OK, sensor.run());

    data = sensor.get_data();

    TEST_ASSERT_EQUAL(reader.value, data.raw);
    TEST_ASSERT_EQUAL(reader.value, data.voltage);
    TEST_ASSERT_EQUAL_DOUBLE(0, data.moisture);
    TEST_ASSERT_EQUAL(SoilStatus::Saturated, data.prev_status);
    TEST_ASSERT_EQUAL(1, data.prev_status_duration);
    TEST_ASSERT_EQUAL(SoilStatus::Error, data.curr_status);
    TEST_ASSERT_EQUAL(1, data.curr_status_duration);
    TEST_ASSERT_EQUAL_DOUBLE(0, data.status_progress);
    TEST_ASSERT_EQUAL(2, data.write_count);

    clock.value += resolution;
    // Still error, threshold hasn't been reached.
    reader.value += (status_interval / 2) - 1;

    TEST_ASSERT_EQUAL(status::StatusCode::OK, sensor.run());

    data = sensor.get_data();

    TEST_ASSERT_EQUAL(reader.value, data.raw);
    TEST_ASSERT_EQUAL(reader.value, data.voltage);
    TEST_ASSERT_EQUAL_DOUBLE(0, data.moisture);
    TEST_ASSERT_EQUAL(SoilStatus::Saturated, data.prev_status);
    TEST_ASSERT_EQUAL(1, data.prev_status_duration);
    TEST_ASSERT_EQUAL(SoilStatus::Error, data.curr_status);
    TEST_ASSERT_EQUAL(2, data.curr_status_duration);
    TEST_ASSERT_EQUAL_DOUBLE(0, data.status_progress);
    TEST_ASSERT_EQUAL(2, data.write_count);

    clock.value += resolution;
    // Leave error, threshold has been reached.
    reader.value += 1;

    expected_moisture -=
        algo::MathOps::round_floor(100 * moisture_per_off * (status_interval / 2), 2);

    TEST_ASSERT_EQUAL(status::StatusCode::OK, sensor.run());

    data = sensor.get_data();

    TEST_ASSERT_EQUAL(reader.value, data.raw);
    TEST_ASSERT_EQUAL(reader.value, data.voltage);
    TEST_ASSERT_EQUAL_DOUBLE(expected_moisture, data.moisture);
    TEST_ASSERT_EQUAL(SoilStatus::Error, data.prev_status);
    TEST_ASSERT_EQUAL(3, data.prev_status_duration);
    TEST_ASSERT_EQUAL(SoilStatus::Saturated, data.curr_status);
    TEST_ASSERT_EQUAL(0, data.curr_status_duration);
    TEST_ASSERT_EQUAL_DOUBLE(50, data.status_progress);
    TEST_ASSERT_EQUAL(3, data.write_count);

    // Back again to error state.
    clock.value += resolution;
    reader.value = def_max + 1;

    TEST_ASSERT_EQUAL(status::StatusCode::OK, sensor.run());

    data = sensor.get_data();

    TEST_ASSERT_EQUAL(reader.value, data.raw);
    TEST_ASSERT_EQUAL(reader.value, data.voltage);
    TEST_ASSERT_EQUAL_DOUBLE(0, data.moisture);
    TEST_ASSERT_EQUAL(SoilStatus::Saturated, data.prev_status);
    TEST_ASSERT_EQUAL(1, data.prev_status_duration);
    TEST_ASSERT_EQUAL(SoilStatus::Error, data.curr_status);
    TEST_ASSERT_EQUAL(0, data.curr_status_duration);
    TEST_ASSERT_EQUAL_DOUBLE(0, data.status_progress);
    TEST_ASSERT_EQUAL(4, data.write_count);
}

} // namespace soil
} // namespace sensor
} // namespace ocs
