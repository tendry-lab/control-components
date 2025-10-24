/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_sensor/ds18b20/resolution_to_str.h"

namespace ocs {
namespace sensor {
namespace ds18b20 {

const char* resolution_to_str(Sensor::Configuration::Resolution resolution) {
    switch (resolution) {
    case Sensor::Configuration::Resolution::Bit_12:
        return "Bit_12";
    case Sensor::Configuration::Resolution::Bit_11:
        return "Bit_11";
    case Sensor::Configuration::Resolution::Bit_10:
        return "Bit_10";
    case Sensor::Configuration::Resolution::Bit_9:
        return "Bit_9";

    default:
        break;
    }

    return "<none>";
}

} // namespace ds18b20
} // namespace sensor
} // namespace ocs
