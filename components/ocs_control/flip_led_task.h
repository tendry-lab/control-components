/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_control/iled.h"
#include "ocs_core/noncopyable.h"
#include "ocs_scheduler/itask.h"

namespace ocs {
namespace control {

class FlipLedTask : public scheduler::ITask, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p led to control LED.
    //!  - @p priority - on each flip LED is locked with @p priority.
    FlipLedTask(ILED& led, ILED::Priority priority);

    //! Flip LED on each task run.
    status::StatusCode run() override;

private:
    const ILED::Priority priority_ { ILED::Priority::None };

    ILED& led_;
};

} // namespace control
} // namespace ocs
