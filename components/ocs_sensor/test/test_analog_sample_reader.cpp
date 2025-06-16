/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <algorithm>
#include <random>
#include <vector>

#include "unity.h"

#include "ocs_sensor/analog_sample_reader.h"
#include "ocs_test/memory_storage.h"

namespace ocs {
namespace sensor {

namespace {

// Ref: https://stackoverflow.com/a/20136256/3873244
template <typename T> T random(T range_from, T range_to) {
    std::random_device rand_dev;
    std::mt19937 generator(rand_dev());
    std::uniform_int_distribution<T> distr(range_from, range_to);
    return distr(generator);
}

using Samples = std::vector<int>;

Samples generate_samples(const AnalogConfig& config) {
    const uint16_t max_value = std::pow(2, static_cast<uint8_t>(config.get_bitwidth()));

    Samples samples;

    for (unsigned n = 0; n < config.get_sample_count(); ++n) {
        samples.push_back(random(static_cast<uint16_t>(0), max_value));
    }

    return samples;
}

int compute_avg(const Samples& samples) {
    return std::accumulate(samples.begin(), samples.end(), 0) / samples.size();
}

} // namespace

namespace {

struct TestDelayer : public system::IRtDelayer, private core::NonCopyable<> {
    status::StatusCode delay(core::Time) {
        return status::StatusCode::OK;
    }
};

class TestAdcReader : public io::adc::IReader, private core::NonCopyable<> {
public:
    TestAdcReader() = default;

    explicit TestAdcReader(const Samples& samples)
        : samples_(samples) {
    }

    status::StatusCode read(int& raw) override {
        if (!samples_.size()) {
            return status::StatusCode::InvalidState;
        }

        raw = samples_.back();
        samples_.pop_back();

        return status::StatusCode::OK;
    }

private:
    Samples samples_;
};

} // namespace

TEST_CASE("Analog sample reader: ADC failed to read",
          "[ocs_sensor], [analog_sample_reader]") {
    const uint16_t def_min = 1;
    const uint16_t def_max = 2;
    const char* id = "test";
    const auto def_oversampling_mode = AnalogConfig::OversamplingMode::Mode_32;

    test::MemoryStorage storage;

    AnalogConfig config(storage, def_min, def_max, static_cast<io::adc::Bitwidth>(10),
                        def_oversampling_mode, id);
    TEST_ASSERT_TRUE(config.valid());

    TestDelayer delayer;
    TestAdcReader adc_reader;
    AnalogSampleReader sample_reader(delayer, adc_reader, config);

    int raw = -1;
    TEST_ASSERT_EQUAL(status::StatusCode::InvalidState, sample_reader.read(raw));
    TEST_ASSERT_EQUAL(-1, raw);
}

TEST_CASE("Analog sample reader: invalid sample count",
          "[ocs_sensor], [analog_sample_reader]") {
    const uint16_t def_min = 1;
    const uint16_t def_max = 2;
    const char* id = "test";
    const auto def_oversampling_mode = AnalogConfig::OversamplingMode::None;

    test::MemoryStorage storage;

    AnalogConfig config(storage, def_min, def_max, static_cast<io::adc::Bitwidth>(10),
                        def_oversampling_mode, id);
    TEST_ASSERT_FALSE(config.valid());

    TestDelayer delayer;
    TestAdcReader adc_reader;
    AnalogSampleReader sample_reader(delayer, adc_reader, config);

    int raw = -1;
    TEST_ASSERT_EQUAL(status::StatusCode::InvalidState, sample_reader.read(raw));
    TEST_ASSERT_EQUAL(-1, raw);
}

TEST_CASE("Analog sample reader: invalid bitwidth",
          "[ocs_sensor], [analog_sample_reader]") {
    const uint16_t def_min = 1;
    const uint16_t def_max = 2;
    const char* id = "test";
    const auto def_oversampling_mode = AnalogConfig::OversamplingMode::Mode_32;

    test::MemoryStorage storage;

    AnalogConfig config(storage, def_min, def_max, static_cast<io::adc::Bitwidth>(0),
                        def_oversampling_mode, id);
    TEST_ASSERT_FALSE(config.valid());

    TestDelayer delayer;
    TestAdcReader adc_reader;
    AnalogSampleReader sample_reader(delayer, adc_reader, config);

    int raw = -1;
    TEST_ASSERT_EQUAL(status::StatusCode::InvalidState, sample_reader.read(raw));
    TEST_ASSERT_EQUAL(-1, raw);
}

TEST_CASE("Analog sample reader: OK", "[ocs_sensor], [analog_sample_reader]") {
    const uint16_t def_min = 1;
    const uint16_t def_max = 2;
    const char* id = "test";
    const auto def_oversampling_mode = AnalogConfig::OversamplingMode::Mode_32;
    const auto def_bitwidth = static_cast<io::adc::Bitwidth>(10);

    test::MemoryStorage storage;

    AnalogConfig config(storage, def_min, def_max, def_bitwidth, def_oversampling_mode,
                        id);
    TEST_ASSERT_TRUE(config.valid());

    const auto samples = generate_samples(config);
    TestAdcReader adc_reader(samples);

    TestDelayer delayer;
    AnalogSampleReader sample_reader(delayer, adc_reader, config);

    int raw = -1;
    TEST_ASSERT_EQUAL(status::StatusCode::OK, sample_reader.read(raw));
    TEST_ASSERT_EQUAL(compute_avg(samples), raw);
}

} // namespace sensor
} // namespace ocs
