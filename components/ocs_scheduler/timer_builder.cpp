/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_scheduler/timer_builder.h"
#include "ocs_scheduler/target_esp32/high_resolution_timer.h"

namespace ocs {
namespace scheduler {

TimerBuilder::ITimerPtr TimerBuilder::make_high_resolution_timer(ITask& task,
                                                                 const char* name,
                                                                 core::Time interval) {
    return ITimerPtr(new (std::nothrow) HighResolutionTimer(task, name, interval));
}

} // namespace scheduler
} // namespace ocs
