/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>
#include <string>

#include "ocs_core/noncopyable.h"
#include "ocs_core/time.h"
#include "ocs_io/adc/istore.h"
#include "ocs_scheduler/itask_scheduler.h"
#include "ocs_sensor/analog_config.h"
#include "ocs_sensor/ldr/analog_sensor.h"

namespace ocs {
namespace sensor {
namespace ldr {

class AnalogSensorPipeline : public core::NonCopyable<> {
public:
    struct Params {
        io::adc::Channel adc_channel { static_cast<io::adc::Channel>(0) };
        core::Time read_interval { 0 };
    };

    //! Initialize.
    //!
    //! @params
    //!  - @p adc_store to configure ADC channels.
    //!  - @p task_scheduler to schedule periodic ADC readings.
    //!  - @p config to read sensor configuration.
    //!  - @p id to distringuish one sensor from another.
    AnalogSensorPipeline(io::adc::IStore& adc_store,
                         scheduler::ITaskScheduler& task_scheduler,
                         const AnalogConfig& config,
                         const char* id,
                         Params params);

    //! Return the underlying sensor.
    AnalogSensor& get_sensor();

private:
    const std::string task_id_;

    io::adc::IStore::IAdcPtr adc_;
    std::unique_ptr<AnalogSensor> sensor_;
};

} // namespace ldr
} // namespace sensor
} // namespace ocs
