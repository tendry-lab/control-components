/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>
#include <string>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "ocs_control/fsm_block_pipeline.h"
#include "ocs_core/noncopyable.h"
#include "ocs_io/adc/istore.h"
#include "ocs_io/gpio/types.h"
#include "ocs_scheduler/itask.h"
#include "ocs_scheduler/itask_scheduler.h"
#include "ocs_sensor/soil/analog_sensor.h"
#include "ocs_storage/storage_builder.h"
#include "ocs_system/fanout_reboot_handler.h"
#include "ocs_system/iclock.h"
#include "ocs_system/irt_delayer.h"

namespace ocs {
namespace sensor {
namespace soil {

//! The sensor is only powered when the relay is activated.
class AnalogRelaySensorPipeline : private core::NonCopyable<> {
public:
    struct Params {
        io::adc::Channel adc_channel { static_cast<io::adc::Channel>(0) };
        control::FsmBlockPipeline::Params fsm_block;
        system::Time read_interval { 0 };
        io::gpio::GpioNum relay_gpio { static_cast<io::gpio::GpioNum>(-1) };
        TickType_t power_on_delay_interval { 0 };
        AnalogSensor::Params sensor;
    };

    //! Initialize.
    AnalogRelaySensorPipeline(system::IClock& clock,
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
    std::unique_ptr<scheduler::ITask> relay_sensor_;
};

} // namespace soil
} // namespace sensor
} // namespace ocs
