/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_io/gpio/ievent_service.h"

namespace ocs {
namespace io {
namespace gpio {

class IsrEventService : public IEventService, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p flags - driver specific configuration flags.
    explicit IsrEventService(int flags);

    //! Start per-pin GPIO events handling.
    status::StatusCode start() override;

    //! Stop per-pin GPIO events handling.
    status::StatusCode stop() override;

    //! Add ISR handler for the GPIO events.
    status::StatusCode add(Gpio gpio, scheduler::IEventHandler& handler) override;

private:
    static void handle_isr_(void* arg);

    const int flags_ { 0 };
};

} // namespace gpio
} // namespace io
} // namespace ocs
