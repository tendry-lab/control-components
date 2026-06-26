/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_system/target_esp32/high_resolution_timer_builder.h"
#include "ocs_system/target_esp32/high_resolution_timer.h"

namespace ocs {
namespace system {

HighResolutionTimerBuilder::HighResolutionTimerBuilder(IArena& arena)
    : arena_(arena) {
}

UniquePtr<ITimer> HighResolutionTimerBuilder::make_timer(scheduler::ITask& task,
                                                         const char* id,
                                                         system::Time interval) {
    return make_unique_ptr<HighResolutionTimer>(arena_, task, id, interval);
}

} // namespace system
} // namespace ocs
