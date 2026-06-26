/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <string>

#include "ocs_core/freertos.h"
#include "ocs_core/noncopyable.h"
#include "ocs_core/spmc_node.h"
#include "ocs_io/i2c/itransceiver.h"
#include "ocs_sensor/sht4x/isensor.h"
#include "ocs_sensor/sht4x/serial_number.h"
#include "ocs_storage/istorage.h"

namespace ocs {
namespace sensor {
namespace sht4x {

class Sensor : public ISensor, private core::NonCopyable<> {
public:
    struct Params {
        //! How long to wait before receiving the operation result from the I2C device.
        TickType_t i2c_delay_interval { pdMS_TO_TICKS(10) };

        //! How long to wait for I2C operation to complete.
        system::Time i2c_wait_timeout { system::Duration::second * 5 };
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

    //! Return the latest data measured by the sensor.
    MeasureData get_measure_data() const override;

    //! Return the sensor heater data.
    HeaterData get_heater_data() const override;

    //! Perform the sensor command.
    status::StatusCode perform(Command command) override;

private:
    static TickType_t estimate_heating_delay_(Command command);

    static constexpr const char* heating_count_key_ = "heating_count";

    status::StatusCode perform_measure_(Command command);
    status::StatusCode perform_reset_();
    status::StatusCode perform_heat_(Command command);
    status::StatusCode receive_data_(MeasureData& data);
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

    uint32_t heating_count_ { 0 };

    core::SpmcNode<MeasureData> measure_data_;
    core::SpmcNode<HeaterData> heater_data_;
};

} // namespace sht4x
} // namespace sensor
} // namespace ocs
