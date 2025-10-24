/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/iformatter.h"
#include "ocs_sensor/sht41/sensor.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

class SHT41SensorFormatter : public fmt::json::IFormatter, private core::NonCopyable<> {
public:
    //! Initialize.
    explicit SHT41SensorFormatter(sensor::sht41::Sensor& sensor);

    //! Format SHT41 sensor data into @p json.
    status::StatusCode format(cJSON* json) override;

private:
    sensor::sht41::Sensor& sensor_;
};

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
