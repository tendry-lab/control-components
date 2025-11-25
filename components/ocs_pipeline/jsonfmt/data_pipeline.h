/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>

#include "ocs_diagnostic/counter_store.h"
#include "ocs_fmt/json/fanout_formatter.h"
#include "ocs_pipeline/basic/system_counter_pipeline.h"
#include "ocs_pipeline/jsonfmt/counter_formatter.h"
#include "ocs_pipeline/jsonfmt/registration_formatter.h"
#include "ocs_pipeline/jsonfmt/telemetry_formatter.h"
#include "ocs_scheduler/itask_scheduler.h"
#include "ocs_storage/storage_builder.h"
#include "ocs_system/fanout_reboot_handler.h"
#include "ocs_system/iclock.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

//! Component to handle telemetry and registration data.
class DataPipeline : private core::NonCopyable<> {
public:
    //! Initialize.
    DataPipeline(system::IClock& clock,
                 storage::StorageBuilder& storage_builder,
                 scheduler::ITaskScheduler& task_scheduler,
                 system::FanoutRebootHandler& reboot_handler,
                 const system::DeviceInfo& device_info);

    fmt::json::FanoutFormatter& get_telemetry_formatter();
    fmt::json::FanoutFormatter& get_registration_formatter();

private:
    std::unique_ptr<TelemetryFormatter> telemetry_formatter_;
    std::unique_ptr<RegistrationFormatter> registration_formatter_;

    std::unique_ptr<storage::IStorage> system_counter_storage_;
    std::unique_ptr<diagnostic::CounterStore> counter_store_;
    std::unique_ptr<CounterFormatter> counter_formatter_;
    std::unique_ptr<basic::SystemCounterPipeline> system_counter_pipeline_;
};

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
