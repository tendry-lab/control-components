/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_sensor/soil/soil_status.h"

namespace ocs {
namespace sensor {
namespace soil {

//! Convert soil moisture status to human-readable description.
const char* soil_status_to_str(SoilStatus);

} // namespace soil
} // namespace sensor
} // namespace ocs
