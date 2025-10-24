/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_sensor/soil/analog_relay_sensor_pipeline.h"
#include "ocs_sensor/analog_sample_reader.h"
#include "ocs_sensor/soil/analog_relay_sensor.h"
#include "ocs_sensor/soil/analog_sensor_pipeline.h"

namespace ocs {
namespace sensor {
namespace soil {

AnalogRelaySensorPipeline::AnalogRelaySensorPipeline(
    system::IClock& clock,
    io::adc::IStore& adc_store,
    io::adc::IConverter& adc_converter,
    storage::StorageBuilder& storage_builder,
    system::IRtDelayer& delayer,
    system::FanoutRebootHandler& reboot_handler,
    scheduler::ITaskScheduler& task_scheduler,
    const AnalogConfig& config,
    const char* id,
    AnalogRelaySensorPipeline::Params params)
    : task_id_(std::string(id) + "_task") {
    adc_reader_ = adc_store.add(params.adc_channel);
    configASSERT(adc_reader_);

    reader_ = adc_reader_.get();

    sample_reader_.reset(new (std::nothrow)
                             AnalogSampleReader(delayer, *reader_, config));
    configASSERT(sample_reader_);

    reader_ = sample_reader_.get();

    fsm_block_pipeline_.reset(new (std::nothrow) control::FsmBlockPipeline(
        clock, reboot_handler, task_scheduler, storage_builder, id, params.fsm_block));
    configASSERT(fsm_block_pipeline_);

    sensor_.reset(new (std::nothrow) AnalogSensor(*reader_, adc_converter,
                                                  fsm_block_pipeline_->get_block(),
                                                  config, params.sensor));
    configASSERT(sensor_);

    relay_sensor_.reset(new (std::nothrow) AnalogRelaySensor(
        *sensor_, params.relay_gpio, params.power_on_delay_interval));
    configASSERT(relay_sensor_);

    configASSERT(
        task_scheduler.add(*relay_sensor_, task_id_.c_str(), params.read_interval)
        == status::StatusCode::OK);
}

AnalogSensor& AnalogRelaySensorPipeline::get_sensor() {
    return *sensor_;
}

} // namespace soil
} // namespace sensor
} // namespace ocs
