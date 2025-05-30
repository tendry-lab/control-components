/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_control/fsm_block.h"
#include "ocs_control/ifsm_handler.h"
#include "ocs_core/noncopyable.h"
#include "ocs_core/spmc_node.h"
#include "ocs_io/adc/iconverter.h"
#include "ocs_io/adc/ireader.h"
#include "ocs_sensor/analog_config.h"
#include "ocs_sensor/soil/soil_status.h"

namespace ocs {
namespace sensor {
namespace soil {

class AnalogSensor : public scheduler::ITask, public core::NonCopyable<> {
public:
    //! Various sensor characteristics.
    struct Data {
        int raw { 0 };
        int voltage { 0 };
        int moisture { 0 };
        SoilStatus prev_status { SoilStatus::None };
        SoilStatus curr_status { SoilStatus::None };
        core::Time prev_status_duration { 0 };
        core::Time curr_status_duration { 0 };
        uint64_t write_count { 0 };
        uint8_t status_progress { 0 };
    };

    //! Initialize.
    //!
    //! @params
    //!  - @p reader to read the ADC value from the sensor.
    //!  - @p converter to convert the ADC reading to voltage.
    //!  - @p fsm_block to measure the soil status duration.
    //!  - @p config to read the sensor configuration.
    AnalogSensor(io::adc::IReader& reader,
                 io::adc::IConverter& converter,
                 control::FsmBlock& fsm_block,
                 const AnalogConfig& config);

    //! Read sensor data.
    status::StatusCode run() override;

    //! Return the latest sensor data.
    Data get_data() const;

private:
    bool is_invalid_input_(int raw) const;
    int calculate_moisture_(int raw) const;
    SoilStatus calculate_status_(int raw) const;
    uint8_t calculate_status_progress_(int raw) const;
    uint16_t get_status_len_() const;

    void update_data_(int raw, int voltage);

    // Saturated, Wet, Depletion, Dry.
    static constexpr SoilStatus statuses_[] = {
        SoilStatus::Saturated,
        SoilStatus::Wet,
        SoilStatus::Depletion,
        SoilStatus::Dry,
    };

    const AnalogConfig& config_;

    io::adc::IReader& reader_;
    io::adc::IConverter& converter_;
    control::FsmBlock& fsm_block_;

    core::SpmcNode<Data> data_;
};

} // namespace soil
} // namespace sensor
} // namespace ocs
