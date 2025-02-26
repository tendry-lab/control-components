/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_sensor/ldr/analog_sensor.h"
#include "ocs_sensor/analog_config.h"

namespace ocs {
namespace sensor {
namespace ldr {

AnalogSensor::AnalogSensor(io::adc::IReader& reader,
                           io::adc::IConverter& converter,
                           const AnalogConfig& config)
    : config_(config)
    , reader_(reader)
    , converter_(converter) {
}

status::StatusCode AnalogSensor::run() {
    if (!config_.valid()) {
        return status::StatusCode::InvalidState;
    }

    int raw = 0;
    auto code = reader_.read(raw);
    if (code != status::StatusCode::OK) {
        return code;
    }

    int voltage = 0;
    code = converter_.convert(voltage, raw);
    if (code != status::StatusCode::OK) {
        return code;
    }

    update_data_(raw, voltage);

    return status::StatusCode::OK;
}

AnalogSensor::Data AnalogSensor::get_data() const {
    return data_.get();
}

int AnalogSensor::calculate_lightness_(int raw) const {
    if (raw >= config_.get_max()) {
        return 100;
    }

    if (raw <= config_.get_min()) {
        return 0;
    }

    const int range = config_.get_max() - config_.get_min();
    const int offset = raw - config_.get_min();
    const float lightness = static_cast<float>(offset) / range;

    return 100 * lightness;
}

void AnalogSensor::update_data_(int raw, int voltage) {
    Data data;

    data.raw = raw;
    data.voltage = voltage;
    data.lightness = calculate_lightness_(raw);

    data_.set(data);
}

} // namespace ldr
} // namespace sensor
} // namespace ocs
