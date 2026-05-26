/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>

#include "ocs_scheduler/itask.h"
#include "ocs_system/iarena.h"
#include "ocs_system/irandomizer.h"
#include "ocs_system/ireboot_handler.h"
#include "ocs_system/irebooter.h"
#include "ocs_system/irt_delayer.h"
#include "ocs_system/itimer.h"
#include "ocs_system/time.h"

namespace ocs {
namespace system {

class PlatformBuilder {
public:
    using IRtDelayerPtr = UniquePtr<IRtDelayer>;
    using IRebooterPtr = UniquePtr<IRebooter>;
    using IRandomizerPtr = UniquePtr<IRandomizer>;
    using ITimerPtr = UniquePtr<ITimer>;

    //! Initialize.
    //!
    //! @params
    //!  - @p arena to perform dynamic allocations.
    explicit PlatformBuilder(IArena& arena);

    //! Create real-time delayer for highly-accurate delays.
    IRtDelayerPtr make_rt_delayer();

    //! Create a basic system rebooter.
    IRebooterPtr make_rebooter(IRebootHandler& handler);

    //! Create a basic system randomizer.
    IRandomizerPtr make_randomizer();

    //! Create a high resolution timer.
    //!
    //!  - @p task to be invoked periodically at the configured interval.
    //!  - @p name to distinguish one timer from another.
    //!  - @p interval - timer interval.
    ITimerPtr make_high_resolution_timer(scheduler::ITask& task,
                                         const char* name,
                                         system::Time interval);

private:
    IArena& arena_;
};

} // namespace system
} // namespace ocs
