/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_scheduler/ievent_handler.h"
#include "ocs_system/irebooter.h"

namespace ocs {
namespace system {

class RebootEventHandler : public scheduler::IEventHandler, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p rebooter to reboot the system when the event occurs.
    explicit RebootEventHandler(IRebooter& rebooter);

    //! Reboot the system when the event occurs.
    status::StatusCode handle_event() override;

private:
    IRebooter& rebooter_;
};

} // namespace system
} // namespace ocs
