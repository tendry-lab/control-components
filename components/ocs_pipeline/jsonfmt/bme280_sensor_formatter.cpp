/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_pipeline/jsonfmt/bme280_sensor_formatter.h"
#include "ocs_fmt/json/cjson_object_formatter.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

BME280SensorFormatter::BME280SensorFormatter(sensor::bme280::Sensor& sensor)
    : sensor_(sensor) {
}

status::StatusCode BME280SensorFormatter::format(cJSON* json) {
    fmt::json::CjsonObjectFormatter formatter(json);

    const auto data = sensor_.get_data();

    if (!formatter.add_number_cs("sensor_bme280_pressure", data.pressure)) {
        return status::StatusCode::NoMem;
    }
    if (!formatter.add_number_cs("sensor_bme280_temperature", data.temperature)) {
        return status::StatusCode::NoMem;
    }
    if (!formatter.add_number_cs("sensor_bme280_humidity", data.humidity)) {
        return status::StatusCode::NoMem;
    }

    return status::StatusCode::OK;
}

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
