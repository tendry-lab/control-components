/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_scheduler/itask.h"
#include "ocs_system/iarena.h"
#include "ocs_system/itimer.h"
#include "ocs_system/time.h"

namespace ocs {
namespace system {

class ITimerBuilder {
public:
    //! Destroy.
    virtual ~ITimerBuilder() = default;

    //! Create a new periodic timer.
    //!
    //! @params
    //!  - @p task to be invoked periodically at the configured interval.
    //!  - @p id to distinguish one timer from another.
    //!  - @p interval - timer period; allowed resolution depends on the implementation.
    virtual UniquePtr<ITimer>
    make_timer(scheduler::ITask& task, const char* id, system::Time interval) = 0;
};

} // namespace system
} // namespace ocs
