/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_io/gpio/types.h"
#include "ocs_scheduler/ievent_handler.h"
#include "ocs_status/code.h"

namespace ocs {
namespace io {
namespace gpio {

class IEventService {
public:
    //! Destroy.
    virtual ~IEventService() = default;

    //! Start handling per-pin GPIO events.
    virtual status::StatusCode start() = 0;

    //! Stop handling per-pin GPIO events.
    virtual status::StatusCode stop() = 0;

    //! Add handler for GPIO state changes.
    //!
    //! @params
    //!  - @p gpio - GPIO number for which to register the handler.
    //!  - @p handler - handler to be called when the GPIO state is changed.
    //!
    //! @remarks
    //!  Handler should be ISR safe.
    virtual status::StatusCode add(Gpio gpio, scheduler::IEventHandler& handler) = 0;
};

} // namespace gpio
} // namespace io
} // namespace ocs
