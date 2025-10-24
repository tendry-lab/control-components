/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_core/spmc_node.h"
#include "ocs_io/adc/iconverter.h"
#include "ocs_io/adc/ireader.h"
#include "ocs_scheduler/itask.h"
#include "ocs_sensor/analog_config.h"

namespace ocs {
namespace sensor {
namespace ldr {

class AnalogSensor : public scheduler::ITask, private core::NonCopyable<> {
public:
    //! Various sensor characteristics.
    struct Data {
        int raw { 0 };
        int voltage { 0 };
        int lightness { 0 };
    };

    //! Initialize.
    //!
    //! @params
    //!  - @p reader to read ADC value from the sensor.
    //!  - @p converter to convert the ADC reading to voltage.
    //!  - @p config to read sensor configuration.
    AnalogSensor(io::adc::IReader& reader,
                 io::adc::IConverter& converter,
                 const AnalogConfig& config);

    //! Read sensor data.
    status::StatusCode run() override;

    //! Return the latest sensor data.
    Data get_data() const;

private:
    int calculate_lightness_(int raw) const;

    void update_data_(int raw, int voltage);

    const AnalogConfig& config_;

    io::adc::IReader& reader_;
    io::adc::IConverter& converter_;
    core::SpmcNode<Data> data_;
};

} // namespace ldr
} // namespace sensor
} // namespace ocs
