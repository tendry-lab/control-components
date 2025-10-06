/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_control/ibutton_handler.h"
#include "ocs_core/noncopyable.h"
#include "ocs_core/static_event_group.h"
#include "ocs_scheduler/ievent_handler.h"
#include "ocs_scheduler/itask.h"

namespace ocs {
namespace control {

class ResetFsm : public IButtonHandler,
                 public scheduler::ITask,
                 private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p handler to be called when the button is pressed (ignored during the FSR).
    //!  - @p release_interval - ensure button is released within the provided interval.
    ResetFsm(scheduler::IEventHandler& handler, core::Time release_interval);

    //! Handle asynchronous button events.
    status::StatusCode run() override;

    //! Dispatch asynchronous button events.
    //!
    //! @remarks
    //!  ISR safe.
    status::StatusCode handle_pressed(core::Time duration) override;

private:
    const core::Time release_interval_ { 0 };

    scheduler::IEventHandler& handler_;

    core::StaticEventGroup event_group_;
};

} // namespace control
} // namespace ocs
