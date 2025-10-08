/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>

#include "ocs_core/noncopyable.h"
#include "ocs_diagnostic/basic_counter_holder.h"
#include "ocs_diagnostic/basic_persistent_counter.h"
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
                          diagnostic::BasicCounterHolder& counter_holder);

private:
    std::unique_ptr<diagnostic::ICounter> uptime_counter_;
    std::unique_ptr<diagnostic::BasicPersistentCounter> uptime_persistent_counter_;

    std::unique_ptr<diagnostic::ICounter> lifetime_counter_;
    std::unique_ptr<diagnostic::BasicPersistentCounter> lifetime_persistent_counter_;
};

} // namespace basic
} // namespace pipeline
} // namespace ocs
