/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/iformatter.h"
#include "ocs_sensor/ldr/analog_sensor.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

class LdrAnalogSensorFormatter : public fmt::json::IFormatter,
                                 private core::NonCopyable<> {
public:
    //! Initialize.
    explicit LdrAnalogSensorFormatter(sensor::ldr::AnalogSensor& sensor);

    //! Format LDR sensor data into @p json.
    status::StatusCode format(cJSON* json) override;

private:
    sensor::ldr::AnalogSensor& sensor_;
};

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
