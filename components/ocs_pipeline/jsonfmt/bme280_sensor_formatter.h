/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/iformatter.h"
#include "ocs_sensor/bme280/sensor.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

class BME280SensorFormatter : public fmt::json::IFormatter, private core::NonCopyable<> {
public:
    //! Initialize.
    explicit BME280SensorFormatter(sensor::bme280::Sensor& sensor);

    //! Format bme280 sensor data into @p json.
    status::StatusCode format(cJSON* json) override;

private:
    sensor::bme280::Sensor& sensor_;
};

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
