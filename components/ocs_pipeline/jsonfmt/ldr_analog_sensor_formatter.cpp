/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_pipeline/jsonfmt/ldr_analog_sensor_formatter.h"
#include "ocs_fmt/json/cjson_object_formatter.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

LdrAnalogSensorFormatter::LdrAnalogSensorFormatter(sensor::ldr::AnalogSensor& sensor)
    : sensor_(sensor) {
}

status::StatusCode LdrAnalogSensorFormatter::format(cJSON* json) {
    fmt::json::CjsonObjectFormatter formatter(json);

    const auto data = sensor_.get_data();

    if (!formatter.add_number_cs("sensor_ldr_raw", data.raw)) {
        return status::StatusCode::NoMem;
    }

    if (!formatter.add_number_cs("sensor_ldr_voltage", data.voltage)) {
        return status::StatusCode::NoMem;
    }

    if (!formatter.add_number_cs("sensor_ldr_lightness", data.lightness)) {
        return status::StatusCode::NoMem;
    }

    return status::StatusCode::OK;
}

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
