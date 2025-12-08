/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <string>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "ocs_core/noncopyable.h"
#include "ocs_core/spmc_node.h"
#include "ocs_io/i2c/itransceiver.h"
#include "ocs_scheduler/itask.h"
#include "ocs_sensor/sht4x/serial_number.h"
#include "ocs_storage/istorage.h"

namespace ocs {
namespace sensor {
namespace sht4x {

//! Read data from SHT4x sensor.
//!
//! @reference
//!  https://sensirion.com/media/documents/33FD6951/6555C40E/Sensirion_Datasheet_SHT4x.pdf
class Sensor : public scheduler::ITask, private core::NonCopyable<> {
public:
    //! Various sensor characteristics.
    struct Data {
        double humidity { 0.0 };
        double temperature { 0.0 };
        size_t heating_count { 0 };
    };

    enum class Command {
        MeasureHighPrecision = 0xFD,
        MeasureMediumPrecision = 0xF6,
        MeasureLowPrecision = 0xE0,
        ReadSerialNumber = 0x89,
        SoftReset = 0x94,
        ActivateHeater_200mW_1000ms = 0x39,
        ActivateHeater_200mW_100ms = 0x32,
        ActivateHeater_110mW_1000ms = 0x2F,
        ActivateHeater_110mW_100ms = 0x24,
        ActivateHeater_20mW_1000ms = 0x1E,
        ActivateHeater_20mW_100ms = 0x15,
    };

    struct Params {
        //! How long to wait before receiving the operation result from the I2C device.
        TickType_t send_wait_interval { pdMS_TO_TICKS(10) };

        //! How long to wait for I2C operation to complete.
        system::Time bus_wait_interval { system::Duration::second * 5 };

        //! How precise data should be measured.
        Command measure_command { Command::MeasureLowPrecision };

        //! Heater activation command.
        Command heating_command { Command::ActivateHeater_20mW_100ms };
    };

    //! Initialize.
    //!
    //! @params
    //!  - @p transceiver to communicate with the I2C device.
    //!  - @p storage to persist number of times the heater was activated.
    //!  - @p id - unique sensor identifier, e.g. sht40 or sht41.
    //!  - @p params - various sensor settings.
    //!
    //! @remarks
    //!  Sensor is reset on initialization.
    Sensor(io::i2c::ITransceiver& transceiver,
           storage::IStorage& storage,
           const char* id,
           Params params);

    //! Read sensor data.
    status::StatusCode run() override;

    //! Return the latest sensor data.
    Data get_data() const;

    //! Reset the sensor.
    //!
    //! @remarks
    //!  Should be called in the same context as run() call.
    status::StatusCode reset();

    //! Activate sensor internal heater.
    //!
    //! @notes
    //!  - Heating stops automatically.
    //!  - A high-precision measurement is done before the heater deactivation.
    //!
    //! @remarks
    //!  - Should be called in the same context as run() call.
    //!
    //!  - The heater is designed for a maximum duty cycle of 10%, meaning the total
    //!    heater-on-time should not be longer than 10% of the sensor’s lifetime.
    status::StatusCode heat();

private:
    static const char* command_to_str_(Command command);
    static TickType_t estimate_heating_delay_(Command command);

    static constexpr const char* heating_count_key_ = "heating_count";

    status::StatusCode initialize_();
    status::StatusCode reset_();
    status::StatusCode heat_();
    status::StatusCode read_serial_number_();
    status::StatusCode receive_data_(Data& data);
    status::StatusCode send_command_(Command command);
    status::StatusCode read_heating_count_();
    status::StatusCode write_heating_count_();

    // Long pulse timings:
    //  - 1100ms - heater-on duration.
    //  - 10ms - high precision measurement duration.
    //  - 50ms - for random delays.
    static constexpr TickType_t heating_long_pulse_delay_ = pdMS_TO_TICKS(1100 + 10 + 50);

    // Short pulse timings:
    //  - 110ms - heater-on duration.
    //  - 10ms - high precision measurement duration.
    //  - 50ms - for random delays.
    static constexpr TickType_t heating_short_pulse_delay_ = pdMS_TO_TICKS(110 + 10 + 50);

    const Params params_;
    const std::string log_tag_;

    io::i2c::ITransceiver& transceiver_;
    storage::IStorage& storage_;

    TickType_t heating_delay_ { 0 };
    size_t heating_count_ { 0 };

    bool initialized_ { false };
    SerialNumber serial_number_ { 0 };

    core::SpmcNode<Data> data_;
};

} // namespace sht4x
} // namespace sensor
} // namespace ocs
