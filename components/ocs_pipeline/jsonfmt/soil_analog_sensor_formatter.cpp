/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_pipeline/jsonfmt/soil_analog_sensor_formatter.h"
#include "ocs_fmt/json/cjson_object_formatter.h"
#include "ocs_sensor/soil/soil_status_to_str.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

SoilAnalogSensorFormatter::SoilAnalogSensorFormatter(sensor::soil::AnalogSensor& sensor)
    : sensor_(sensor) {
}

status::StatusCode SoilAnalogSensorFormatter::format(cJSON* json) {
    fmt::json::CjsonObjectFormatter formatter(json);

    const auto data = sensor_.get_data();

    if (!formatter.add_number_cs("sensor_soil_raw", data.raw)) {
        return status::StatusCode::NoMem;
    }

    if (!formatter.add_number_cs("sensor_soil_voltage", data.voltage)) {
        return status::StatusCode::NoMem;
    }

    if (!formatter.add_number_cs("sensor_soil_moisture", data.moisture)) {
        return status::StatusCode::NoMem;
    }

    if (!formatter.add_string_ref_cs(
            "sensor_soil_prev_status",
            sensor::soil::soil_status_to_str(data.prev_status))) {
        return status::StatusCode::NoMem;
    }

    if (!formatter.add_number_cs("sensor_soil_prev_status_dur",
                                 data.prev_status_duration)) {
        return status::StatusCode::NoMem;
    }

    if (!formatter.add_string_ref_cs(
            "sensor_soil_curr_status",
            sensor::soil::soil_status_to_str(data.curr_status))) {
        return status::StatusCode::NoMem;
    }

    if (!formatter.add_number_cs("sensor_soil_curr_status_dur",
                                 data.curr_status_duration)) {
        return status::StatusCode::NoMem;
    }

    if (!formatter.add_number_cs("sensor_soil_write_count", data.write_count)) {
        return status::StatusCode::NoMem;
    }

    if (!formatter.add_number_cs("sensor_soil_status_progress", data.status_progress)) {
        return status::StatusCode::NoMem;
    }

    return status::StatusCode::OK;
}

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
