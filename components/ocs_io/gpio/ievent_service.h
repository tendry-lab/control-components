/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
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
    //!  - @p gpio_num - GPIO number for which to register the handler.
    //!  - @p handler - handler to be called when the GPIO state is changed.
    //!
    //! @remarks
    //!  Handler should be ISR safe.
    virtual status::StatusCode add(GpioNum gpio, scheduler::IEventHandler& handler) = 0;
};

} // namespace gpio
} // namespace io
} // namespace ocs
