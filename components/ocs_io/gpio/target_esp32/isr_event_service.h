/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
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

    //! Add task for the GPIO events.
    //!
    //! @remarks
    //!  @p task should be ISR safe.
    status::StatusCode add(scheduler::ITask& task, GpioNum gpio_num) override;

private:
    static void handle_isr_(void* arg);

    const int flags_ { 0 };
};

} // namespace gpio
} // namespace io
} // namespace ocs
