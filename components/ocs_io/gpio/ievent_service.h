/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_io/gpio/types.h"
#include "ocs_scheduler/itask.h"
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

    //! Add task to be run when the GPIO state changes.
    //!
    //! @params
    //!  - @p task - task to be called when the GPIO state changes.
    //!  - @p gpio_num - GPIO number for which to register the handler.
    virtual status::StatusCode add(scheduler::ITask& task, GpioNum gpio_num) = 0;
};

} // namespace gpio
} // namespace io
} // namespace ocs
