/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_scheduler/event_func_scheduler.h"

namespace ocs {
namespace scheduler {

EventFuncScheduler::EventFuncScheduler(IFuncScheduler& func_scheduler,
                                       EventGroupHandle_t handle,
                                       EventBits_t event)
    : event_(event)
    , func_scheduler_(func_scheduler)
    , handle_(handle) {
    configASSERT(event_);
    configASSERT(handle_);
}

EventFuncScheduler::FuturePtr EventFuncScheduler::add(Func func) {
    auto future = func_scheduler_.add(func);
    if (!future) {
        return nullptr;
    }

    xEventGroupSetBits(handle_, event_);

    return future;
}

} // namespace scheduler
} // namespace ocs
