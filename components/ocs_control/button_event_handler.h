/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_control/ibutton.h"
#include "ocs_control/ibutton_handler.h"
#include "ocs_core/noncopyable.h"
#include "ocs_scheduler/ievent_handler.h"
#include "ocs_system/iclock.h"

namespace ocs {
namespace control {

class ButtonEventHandler : public scheduler::IEventHandler, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p clock to measure for how long the button was pressed.
    //!  - @p button to check if the button is pressed.
    //!  - @p handler to be called when the button is pressed and then released.
    ButtonEventHandler(system::IClock& clock, IButton& button, IButtonHandler& handler);

    //! Handle button state changes.
    //!
    //! @remarks
    //!  ISR safe.
    status::StatusCode handle_event() override;

private:
    system::IClock& clock_;
    IButton& button_;
    IButtonHandler& handler_;

    bool was_pressed_ { false };
    system::Time pressed_ts_ { 0 };
};

} // namespace control
} // namespace ocs
