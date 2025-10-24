/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/iformatter.h"
#include "ocs_sensor/soil/analog_sensor.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

class SoilAnalogSensorFormatter : public fmt::json::IFormatter,
                                  private core::NonCopyable<> {
public:
    //! Initialize.
    explicit SoilAnalogSensorFormatter(sensor::soil::AnalogSensor& sensor);

    //! Format soil sensor data into @p json.
    status::StatusCode format(cJSON* json) override;

private:
    sensor::soil::AnalogSensor& sensor_;
};

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
