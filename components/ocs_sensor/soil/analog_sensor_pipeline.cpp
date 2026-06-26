/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_sensor/soil/analog_sensor_pipeline.h"
#include "ocs_core/freertos.h"
#include "ocs_sensor/analog_sample_reader.h"

namespace ocs {
namespace sensor {
namespace soil {

AnalogSensorPipeline::AnalogSensorPipeline(system::IClock& clock,
                                           io::adc::IStore& adc_store,
                                           io::adc::IConverter& adc_converter,
                                           storage::IStorage& storage,
                                           system::IArena& arena,
                                           system::IRtDelayer& delayer,
                                           system::FanoutRebootHandler& reboot_handler,
                                           scheduler::ITaskScheduler& task_scheduler,
                                           const AnalogConfig& config,
                                           const char* id,
                                           AnalogSensorPipeline::Params params) {
    adc_reader_ = adc_store.add(params.adc_channel);
    configASSERT(adc_reader_);

    reader_ = adc_reader_.get();

    sample_reader_ = ocs::system::make_unique_ptr<AnalogSampleReader>(arena, delayer,
                                                                      *reader_, config);
    configASSERT(sample_reader_);

    reader_ = sample_reader_.get();

    configASSERT(reader_);

    fsm_block_pipeline_ = ocs::system::make_unique_ptr<control::FsmBlockPipeline>(
        arena, arena, clock, reboot_handler, task_scheduler, storage, id,
        params.fsm_block);
    configASSERT(fsm_block_pipeline_);

    sensor_ = ocs::system::make_unique_ptr<AnalogSensor>(arena, *reader_, adc_converter,
                                                         fsm_block_pipeline_->get_block(),
                                                         config, params.sensor);
    configASSERT(sensor_);
}

AnalogSensor& AnalogSensorPipeline::get_sensor() {
    return *sensor_;
}

} // namespace soil
} // namespace sensor
} // namespace ocs
