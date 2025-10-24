/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_sensor/soil/soil_status_to_str.h"

namespace ocs {
namespace sensor {
namespace soil {

const char* soil_status_to_str(SoilStatus status) {
    switch (status) {
    case SoilStatus::Saturated:
        return "Saturated";

    case SoilStatus::Wet:
        return "Wet";

    case SoilStatus::Depletion:
        return "Depletion";

    case SoilStatus::Dry:
        return "Dry";

    case SoilStatus::Error:
        return "Error";

    default:
        break;
    }

    return "<none>";
}

} // namespace soil
} // namespace sensor
} // namespace ocs
