/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/iformatter.h"
#include "ocs_sensor/sht41/sensor.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

class SHT41SensorFormatter : public fmt::json::IFormatter, public core::NonCopyable<> {
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
