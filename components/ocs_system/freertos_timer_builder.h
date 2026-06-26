/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_system/iarena.h"
#include "ocs_system/itimer_builder.h"

namespace ocs {
namespace system {

class FreeRtosTimerBuilder : public ITimerBuilder, private core::NonCopyable<> {
public:
    //! Initialize.
    explicit FreeRtosTimerBuilder(IArena& arena);

    //! Create a FreeRTOS software timer.
    //!
    //! @remarks
    //!  Allowed resolution: one FreeRTOS tick (typically 1 ms).
    UniquePtr<ITimer>
    make_timer(scheduler::ITask& task, const char* id, system::Time interval) override;

private:
    IArena& arena_;
};

} // namespace system
} // namespace ocs
