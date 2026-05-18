/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_sensor/sht4x/command_to_str.h"

namespace ocs {
namespace sensor {
namespace sht4x {

const char* command_to_str(ISensor::Command command) {
    switch (command) {
    case ISensor::Command::MeasureHighPrecision:
        return "measure_high_precision";

    case ISensor::Command::MeasureMediumPrecision:
        return "measure_medium_precision";

    case ISensor::Command::MeasureLowPrecision:
        return "measure_low_precision";

    case ISensor::Command::SoftReset:
        return "soft_reset";

    case ISensor::Command::ActivateHeater_200mW_1000ms:
        return "activate_heater_200mW_1000ms";

    case ISensor::Command::ActivateHeater_200mW_100ms:
        return "activate_heater_200mW_100ms";

    case ISensor::Command::ActivateHeater_110mW_1000ms:
        return "activate_heater_110mW_1000ms";

    case ISensor::Command::ActivateHeater_110mW_100ms:
        return "activate_heater_110mW_100ms";

    case ISensor::Command::ActivateHeater_20mW_1000ms:
        return "activate_heater_20mW_1000ms";

    case ISensor::Command::ActivateHeater_20mW_100ms:
        return "activate_heater_20mW_100ms";
    }

    return "<none>";
}

} // namespace sht4x
} // namespace sensor
} // namespace ocs
