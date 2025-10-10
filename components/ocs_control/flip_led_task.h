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
#include "ocs_scheduler/ievent_handler.h"
#include "ocs_scheduler/itask.h"

namespace ocs {
namespace control {

class FlipLedTask : public scheduler::ITask, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p handler to be called when LED is flipped required number of times.
    //!  - @p led to control LED.
    //!  - @p priority - on each flip LED is locked with @p priority.
    //!  - @p flip_count - number of times the LED should blink.
    FlipLedTask(scheduler::IEventHandler& handler,
                ILed& led,
                ILed::Priority priority,
                unsigned flip_count);

    //! Flip LED on each task run.
    //!
    //! @remarks
    //!  - When LED is blinked the required number of times, the event handler is called.
    //!    After the handler handles the event, the LED task should not be used, ensure to
    //!    remove the LED task from the corresponding task scheduler.
    //!
    //!  - LED isn't automatically unlocked after it blinks the required number of times.
    status::StatusCode run() override;

private:
    const ILed::Priority priority_ { ILed::Priority::None };

    unsigned flip_count_ { 0 };

    scheduler::IEventHandler& handler_;
    ILed& led_;
};

} // namespace control
} // namespace ocs
