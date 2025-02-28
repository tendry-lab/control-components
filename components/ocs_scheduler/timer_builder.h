/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>

#include "ocs_core/time.h"
#include "ocs_scheduler/itask.h"
#include "ocs_scheduler/itimer.h"

namespace ocs {
namespace scheduler {

struct TimerBuilder {
    using ITimerPtr = std::unique_ptr<ITimer>;

    //! Create a high resolution timer.
    //!
    //!  - @p task to be invoked periodically at the configured interval.
    //!  - @p name to distinguish one timer from another.
    //!  - @p interval - timer interval.
    static ITimerPtr
    make_high_resolution_timer(ITask& task, const char* name, core::Time interval);
};

} // namespace scheduler
} // namespace ocs
