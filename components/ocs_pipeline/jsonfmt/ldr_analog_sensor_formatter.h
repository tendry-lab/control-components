/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
