/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_core/spmc_node.h"
#include "ocs_io/adc/iadc.h"
#include "ocs_scheduler/itask.h"
#include "ocs_sensor/analog_config.h"

namespace ocs {
namespace sensor {
namespace ldr {

class AnalogSensor : public scheduler::ITask, public core::NonCopyable<> {
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
    //!  - @p adc to read ADC value from the sensor.
    //!  - @p config to read sensor configuration.
    AnalogSensor(io::adc::IAdc& adc, const AnalogConfig& config);

    //! Read sensor data.
    status::StatusCode run() override;

    //! Return the latest sensor data.
    Data get_data() const;

private:
    int calculate_lightness_(int raw) const;

    void update_data_(int raw, int voltage);

    const AnalogConfig& config_;

    io::adc::IAdc& adc_;
    core::SpmcNode<Data> data_;
};

} // namespace ldr
} // namespace sensor
} // namespace ocs
