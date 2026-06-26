/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_system/freertos_timer_builder.h"
#include "ocs_system/freertos_timer.h"

namespace ocs {
namespace system {

FreeRtosTimerBuilder::FreeRtosTimerBuilder(IArena& arena)
    : arena_(arena) {
}

UniquePtr<ITimer> FreeRtosTimerBuilder::make_timer(scheduler::ITask& task,
                                                   const char* id,
                                                   system::Time interval) {
    return make_unique_ptr<FreeRtosTimer>(arena_, task, id, interval);
}

} // namespace system
} // namespace ocs
