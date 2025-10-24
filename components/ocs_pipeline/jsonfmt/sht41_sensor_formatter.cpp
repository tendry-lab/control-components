/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_pipeline/jsonfmt/sht41_sensor_formatter.h"
#include "ocs_fmt/json/cjson_object_formatter.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

SHT41SensorFormatter::SHT41SensorFormatter(sensor::sht41::Sensor& sensor)
    : sensor_(sensor) {
}

status::StatusCode SHT41SensorFormatter::format(cJSON* json) {
    fmt::json::CjsonObjectFormatter formatter(json);

    const auto data = sensor_.get_data();

    if (!formatter.add_number_cs("sensor_sht41_humidity", data.humidity)) {
        return status::StatusCode::NoMem;
    }

    if (!formatter.add_number_cs("sensor_sht41_temperature", data.temperature)) {
        return status::StatusCode::NoMem;
    }

    if (!formatter.add_number_cs("sensor_sht41_heating_count", data.heating_count)) {
        return status::StatusCode::NoMem;
    }

    return status::StatusCode::OK;
}

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
