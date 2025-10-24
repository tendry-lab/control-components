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

PlatformBuilder::IRtDelayerPtr PlatformBuilder::make_rt_delayer() {
    return IRtDelayerPtr(new (std::nothrow) RtDelayer());
}

PlatformBuilder::IRebooterPtr PlatformBuilder::make_rebooter(IRebootHandler& handler) {
    return IRebooterPtr(new (std::nothrow) Rebooter(handler));
}

PlatformBuilder::IRandomizerPtr PlatformBuilder::make_randomizer() {
    return IRandomizerPtr(new (std::nothrow) Randomizer());
}

PlatformBuilder::ITimerPtr PlatformBuilder::make_high_resolution_timer(
    scheduler::ITask& task, const char* name, system::Time interval) {
    return ITimerPtr(new (std::nothrow) HighResolutionTimer(task, name, interval));
}

} // namespace system
} // namespace ocs
