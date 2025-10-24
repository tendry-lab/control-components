/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <vector>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "ocs_core/noncopyable.h"

namespace ocs {
namespace system {

class SystemStateBuilder : private core::NonCopyable<> {
public:
    struct State {
        configRUN_TIME_COUNTER_TYPE total_time { 0 };
        std::vector<TaskStatus_t> states;
    };

    //! Initialize.
    SystemStateBuilder();

    //! Return the state of each task in the system.
    const State& get() const;

private:
    State state_;
};

} // namespace system
} // namespace ocs
