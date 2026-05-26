/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_system/platform_builder.h"
#include "ocs_system/target_esp32/high_resolution_timer.h"
#include "ocs_system/target_esp32/randomizer.h"
#include "ocs_system/target_esp32/rebooter.h"
#include "ocs_system/target_esp32/rt_delayer.h"

namespace ocs {
namespace system {

PlatformBuilder::PlatformBuilder(IArena& arena)
    : arena_(arena) {
}

PlatformBuilder::IRtDelayerPtr PlatformBuilder::make_rt_delayer() {
    return ocs::system::make_unique_ptr<RtDelayer>(arena_);
}

PlatformBuilder::IRebooterPtr PlatformBuilder::make_rebooter(IRebootHandler& handler) {
    return ocs::system::make_unique_ptr<Rebooter>(arena_, handler);
}

PlatformBuilder::IRandomizerPtr PlatformBuilder::make_randomizer() {
    return ocs::system::make_unique_ptr<Randomizer>(arena_);
}

PlatformBuilder::ITimerPtr PlatformBuilder::make_high_resolution_timer(
    scheduler::ITask& task, const char* name, system::Time interval) {
    return ocs::system::make_unique_ptr<HighResolutionTimer>(arena_, task, name,
                                                             interval);
}

} // namespace system
} // namespace ocs
