/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_sensor/analog_sample_reader.h"
#include "ocs_sensor/soil/analog_sensor_pipeline.h"

namespace ocs {
namespace sensor {
namespace soil {

AnalogSensorPipeline::AnalogSensorPipeline(system::IClock& clock,
                                           io::adc::IStore& adc_store,
                                           io::adc::IConverter& adc_converter,
                                           storage::StorageBuilder& storage_builder,
                                           system::IRtDelayer& delayer,
                                           system::FanoutRebootHandler& reboot_handler,
                                           scheduler::ITaskScheduler& task_scheduler,
                                           const AnalogConfig& config,
                                           const char* id,
                                           AnalogSensorPipeline::Params params)
    : task_id_(std::string(id) + "_task") {
    adc_reader_ = adc_store.add(params.adc_channel);
    configASSERT(adc_reader_);

    reader_ = adc_reader_.get();

    sample_reader_.reset(new (std::nothrow)
                             AnalogSampleReader(delayer, *reader_, config));
    configASSERT(sample_reader_);

    reader_ = sample_reader_.get();

    configASSERT(reader_);

    fsm_block_pipeline_.reset(new (std::nothrow) control::FsmBlockPipeline(
        clock, reboot_handler, task_scheduler, storage_builder, id, params.fsm_block));
    configASSERT(fsm_block_pipeline_);

    sensor_.reset(new (std::nothrow) AnalogSensor(*reader_, adc_converter,
                                                  fsm_block_pipeline_->get_block(),
                                                  config, params.sensor));
    configASSERT(sensor_);

    configASSERT(task_scheduler.add(*sensor_, task_id_.c_str(), params.read_interval)
                 == status::StatusCode::OK);
}

AnalogSensor& AnalogSensorPipeline::get_sensor() {
    return *sensor_;
}

} // namespace soil
} // namespace sensor
} // namespace ocs
