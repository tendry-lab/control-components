/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>

#include "ocs_scheduler/itask.h"
#include "ocs_system/irandomizer.h"
#include "ocs_system/ireboot_handler.h"
#include "ocs_system/irebooter.h"
#include "ocs_system/irt_delayer.h"
#include "ocs_system/itimer.h"
#include "ocs_system/time.h"

namespace ocs {
namespace system {

struct PlatformBuilder {
    using IRtDelayerPtr = std::unique_ptr<IRtDelayer>;
    using IRebooterPtr = std::unique_ptr<IRebooter>;
    using IRandomizerPtr = std::unique_ptr<IRandomizer>;
    using ITimerPtr = std::unique_ptr<ITimer>;

    //! Create real-time delayer for highly-accurate delays.
    static IRtDelayerPtr make_rt_delayer();

    //! Create a basic system rebooter.
    static IRebooterPtr make_rebooter(IRebootHandler& handler);

    //! Create a basic system randomizer.
    static IRandomizerPtr make_randomizer();

    //! Create a high resolution timer.
    //!
    //!  - @p task to be invoked periodically at the configured interval.
    //!  - @p name to distinguish one timer from another.
    //!  - @p interval - timer interval.
    static ITimerPtr make_high_resolution_timer(scheduler::ITask& task,
                                                const char* name,
                                                system::Time interval);
};

} // namespace system
} // namespace ocs
