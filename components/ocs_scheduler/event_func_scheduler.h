/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/freertos.h"
#include "ocs_core/noncopyable.h"
#include "ocs_scheduler/ifunc_scheduler.h"

namespace ocs {
namespace scheduler {

//! Notify the func scheduler owner each time a func is scheduled.
//!
//! @remarks
//!  Typical usage is to attach the func scheduler to the AsyncTaskScheduler, and to
//!  use the received event to wake up the task scheduler, so the scheduled funcs are
//!  handled without delay.
class EventFuncScheduler : public IFuncScheduler, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p func_scheduler to schedule funcs.
    //!  - @p handle to post asynchronous events.
    //!  - @p event to post to the event group after each successfully scheduled func.
    EventFuncScheduler(IFuncScheduler& func_scheduler,
                       EventGroupHandle_t handle,
                       EventBits_t event);

    //! Schedule @p func and post the event.
    //!
    //! @remarks
    //!  The event isn't posted if @p func can't be scheduled.
    FuturePtr add(Func func) override;

private:
    const EventBits_t event_ { 0 };

    IFuncScheduler& func_scheduler_;
    EventGroupHandle_t handle_ { nullptr };
};

} // namespace scheduler
} // namespace ocs
