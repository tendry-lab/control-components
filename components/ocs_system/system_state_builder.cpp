/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_system/system_state_builder.h"

namespace ocs {
namespace system {

SystemStateBuilder::SystemStateBuilder() {
    auto task_count = uxTaskGetNumberOfTasks();
    configASSERT(task_count);

    state_.states.resize(task_count);

    task_count =
        uxTaskGetSystemState(state_.states.data(), task_count, &state_.total_time);
    configASSERT(task_count);
}

const SystemStateBuilder::State& SystemStateBuilder::get() const {
    return state_;
}

} // namespace system
} // namespace ocs
