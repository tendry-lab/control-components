/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_sensor/ds18b20/sensor.h"

namespace ocs {
namespace sensor {
namespace ds18b20 {

//! Return human-readable resolution description.
const char* resolution_to_str(Sensor::Configuration::Resolution resolution);

} // namespace ds18b20
} // namespace sensor
} // namespace ocs
