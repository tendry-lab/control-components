/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>
#include <string>

#include "ocs_core/noncopyable.h"
#include "ocs_io/adc/istore.h"
#include "ocs_scheduler/itask_scheduler.h"
#include "ocs_sensor/analog_config.h"
#include "ocs_sensor/ldr/analog_sensor.h"
#include "ocs_system/irt_delayer.h"
#include "ocs_system/time.h"

namespace ocs {
namespace sensor {
namespace ldr {

class AnalogSensorPipeline : private core::NonCopyable<> {
public:
    struct Params {
        io::adc::Channel adc_channel { static_cast<io::adc::Channel>(0) };
        system::Time read_interval { 0 };
    };

    //! Initialize.
    //!
    //! @params
    //!  - @p adc_store to configure ADC channels.
    //!  - @p adc_converter to convert the ADC reading to voltage.
    //!  - @p task_scheduler to schedule periodic ADC readings.
    //!  - @p config to read sensor configuration.
    //!  - @p id to distringuish one sensor from another.
    AnalogSensorPipeline(system::IRtDelayer& delayer,
                         io::adc::IStore& adc_store,
                         io::adc::IConverter& adc_converter,
                         scheduler::ITaskScheduler& task_scheduler,
                         const AnalogConfig& config,
                         const char* id,
                         Params params);

    //! Return the underlying sensor.
    AnalogSensor& get_sensor();

private:
    const std::string task_id_;

    io::adc::IStore::IReaderPtr adc_reader_;
    std::unique_ptr<io::adc::IReader> sample_reader_;
    io::adc::IReader* reader_ { nullptr };
    std::unique_ptr<AnalogSensor> sensor_;
};

} // namespace ldr
} // namespace sensor
} // namespace ocs
