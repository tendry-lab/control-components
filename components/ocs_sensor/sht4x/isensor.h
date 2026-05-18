/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstdint>

#include "ocs_status/code.h"

namespace ocs {
namespace sensor {
namespace sht4x {

//! SHT4x sensor control.
class ISensor {
public:
    struct MeasureData {
        float humidity { 0.0 };
        float temperature { 0.0 };
    };
    static_assert(sizeof(MeasureData) == 8);

    struct HeaterData {
        uint32_t heating_count { 0 };
    };

    //! Sensor command.
    //!
    //! @reference
    //!  Manual: 4.5 Command Overview, page 12.
    enum class Command : uint8_t {
        MeasureHighPrecision = 0xFD,
        MeasureMediumPrecision = 0xF6,
        MeasureLowPrecision = 0xE0,
        SoftReset = 0x94,
        ActivateHeater_200mW_1000ms = 0x39,
        ActivateHeater_200mW_100ms = 0x32,
        ActivateHeater_110mW_1000ms = 0x2F,
        ActivateHeater_110mW_100ms = 0x24,
        ActivateHeater_20mW_1000ms = 0x1E,
        ActivateHeater_20mW_100ms = 0x15,
    };

    //! Destroy.
    virtual ~ISensor() = default;

    //! Return the latest data measured by the sensor.
    //!
    //! @remarks
    //!  Implementation should be thread-safe.
    virtual MeasureData get_measure_data() const = 0;

    //! Return the sensor heater data.
    //!
    //! @remarks
    //!  Implementation should be thread-safe.
    virtual HeaterData get_heater_data() const = 0;

    //! Perform the sensor command.
    virtual status::StatusCode perform(Command command) = 0;
};

} // namespace sht4x
} // namespace sensor
} // namespace ocs
