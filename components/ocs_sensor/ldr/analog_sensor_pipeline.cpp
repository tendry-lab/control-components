/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_sensor/analog_sample_reader.h"
#include "ocs_sensor/ldr/analog_sensor_pipeline.h"

namespace ocs {
namespace sensor {
namespace ldr {

AnalogSensorPipeline::AnalogSensorPipeline(system::IRtDelayer& delayer,
                                           io::adc::IStore& adc_store,
                                           io::adc::IConverter& adc_converter,
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

    sensor_.reset(new (std::nothrow) AnalogSensor(*reader_, adc_converter, config));
    configASSERT(sensor_);

    configASSERT(task_scheduler.add(*sensor_, task_id_.c_str(), params.read_interval)
                 == status::StatusCode::OK);
}

AnalogSensor& AnalogSensorPipeline::get_sensor() {
    return *sensor_;
}

} // namespace ldr
} // namespace sensor
} // namespace ocs
