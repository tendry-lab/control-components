/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_pipeline/jsonfmt/data_pipeline.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

DataPipeline::DataPipeline(system::IClock& clock,
                           storage::StorageBuilder& storage_builder,
                           scheduler::ITaskScheduler& task_scheduler,
                           system::FanoutRebootHandler& reboot_handler,
                           const system::DeviceInfo& device_info) {
    telemetry_formatter_.reset(new (std::nothrow) TelemetryFormatter());
    configASSERT(telemetry_formatter_);

    registration_formatter_.reset(new (std::nothrow) RegistrationFormatter(device_info));
    configASSERT(registration_formatter_);

    system_counter_storage_ = storage_builder.make("system_counter");
    configASSERT(system_counter_storage_);

    counter_formatter_.reset(new (std::nothrow) CounterFormatter());
    configASSERT(counter_formatter_);

    system_counter_pipeline_.reset(new (std::nothrow) basic::SystemCounterPipeline(
        clock, *system_counter_storage_, reboot_handler, task_scheduler,
        *counter_formatter_));
    configASSERT(system_counter_pipeline_);

    telemetry_formatter_->get_fanout_formatter().add(*counter_formatter_);
}

fmt::json::FanoutFormatter& DataPipeline::get_telemetry_formatter() {
    return telemetry_formatter_->get_fanout_formatter();
}

fmt::json::FanoutFormatter& DataPipeline::get_registration_formatter() {
    return registration_formatter_->get_fanout_formatter();
}

diagnostic::BasicCounterHolder& DataPipeline::get_counter_holder() {
    return *counter_formatter_;
}

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
