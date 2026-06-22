/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_diagnostic/acc_persistent_counter.h"
#include "ocs_diagnostic/mem_persistent_counter.h"
#include "ocs_diagnostic/time_counter.h"
#include "ocs_pipeline/basic/system_counter_pipeline.h"

namespace ocs {
namespace pipeline {
namespace basic {

SystemCounterPipeline::SystemCounterPipeline(system::IArena& arena,
                                             system::IClock& clock,
                                             storage::IStorage& storage,
                                             system::FanoutRebootHandler& reboot_handler,
                                             scheduler::ITaskScheduler& task_scheduler,
                                             diagnostic::CounterStore& counter_store) {
    uptime_counter_ = ocs::system::make_unique_ptr<diagnostic::TimeCounter>(
        arena, clock, "c_sys_uptime", system::Duration::second);
    configASSERT(uptime_counter_);

    uptime_persistent_counter_ =
        ocs::system::make_unique_ptr<diagnostic::MemPersistentCounter>(arena, storage,
                                                                       *uptime_counter_);
    configASSERT(uptime_persistent_counter_);

    reboot_handler.add(*uptime_persistent_counter_);
    counter_store.add(*uptime_persistent_counter_);

    lifetime_counter_ = ocs::system::make_unique_ptr<diagnostic::TimeCounter>(
        arena, clock, "c_sys_lifetime", system::Duration::second);
    configASSERT(lifetime_counter_);

    lifetime_persistent_counter_ =
        ocs::system::make_unique_ptr<diagnostic::AccPersistentCounter>(
            arena, storage, *lifetime_counter_);
    configASSERT(lifetime_persistent_counter_);

    configASSERT(task_scheduler.add(*lifetime_persistent_counter_, "c_sys_lifetime",
                                    system::Duration::hour)
                 == status::StatusCode::OK);

    reboot_handler.add(*lifetime_persistent_counter_);
    counter_store.add(*lifetime_persistent_counter_);
}

} // namespace basic
} // namespace pipeline
} // namespace ocs
