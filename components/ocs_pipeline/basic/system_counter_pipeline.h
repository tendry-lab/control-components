/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>

#include "ocs_core/noncopyable.h"
#include "ocs_diagnostic/basic_persistent_counter.h"
#include "ocs_diagnostic/counter_store.h"
#include "ocs_scheduler/itask_scheduler.h"
#include "ocs_storage/istorage.h"
#include "ocs_system/fanout_reboot_handler.h"
#include "ocs_system/iclock.h"

namespace ocs {
namespace pipeline {
namespace basic {

//! Pipeline to initialize system-wide counters.
class SystemCounterPipeline : private core::NonCopyable<> {
public:
    //! Initilize counters.
    SystemCounterPipeline(system::IClock& clock,
                          storage::IStorage& storage,
                          system::FanoutRebootHandler& reboot_handler,
                          scheduler::ITaskScheduler& task_scheduler,
                          diagnostic::CounterStore& counter_store);

private:
    std::unique_ptr<diagnostic::ICounter> uptime_counter_;
    std::unique_ptr<diagnostic::BasicPersistentCounter> uptime_persistent_counter_;

    std::unique_ptr<diagnostic::ICounter> lifetime_counter_;
    std::unique_ptr<diagnostic::BasicPersistentCounter> lifetime_persistent_counter_;
};

} // namespace basic
} // namespace pipeline
} // namespace ocs
