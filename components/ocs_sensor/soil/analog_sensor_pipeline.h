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

#include "ocs_control/fsm_block_pipeline.h"
#include "ocs_core/noncopyable.h"
#include "ocs_io/adc/istore.h"
#include "ocs_scheduler/itask.h"
#include "ocs_scheduler/itask_scheduler.h"
#include "ocs_sensor/soil/analog_sensor.h"
#include "ocs_system/fanout_reboot_handler.h"
#include "ocs_system/iclock.h"
#include "ocs_system/irt_delayer.h"

namespace ocs {
namespace sensor {
namespace soil {

//! Periodically read the soil moisture data.
class AnalogSensorPipeline : private core::NonCopyable<> {
public:
    struct Params {
        io::adc::Channel adc_channel { static_cast<io::adc::Channel>(0) };
        control::FsmBlockPipeline::Params fsm_block;
        system::Time read_interval { 0 };
        AnalogSensor::Params sensor;
    };

    //! Initialize.
    AnalogSensorPipeline(system::IClock& clock,
                         io::adc::IStore& adc_store,
                         io::adc::IConverter& adc_converter,
                         storage::StorageBuilder& storage_builder,
                         system::IRtDelayer& delayer,
                         system::FanoutRebootHandler& reboot_handler,
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
    std::unique_ptr<control::FsmBlockPipeline> fsm_block_pipeline_;
    std::unique_ptr<AnalogSensor> sensor_;
};

} // namespace soil
} // namespace sensor
} // namespace ocs
