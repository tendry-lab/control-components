/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_sensor/sht4x/isensor.h"

namespace ocs {
namespace sensor {
namespace sht4x {

//! Return human-readble SHT4x sensor command description.
const char* command_to_str(ISensor::Command command);

} // namespace sht4x
} // namespace sensor
} // namespace ocs
