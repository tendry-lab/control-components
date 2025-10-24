/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_scheduler/async_func.h"

namespace ocs {
namespace scheduler {

AsyncFunc::AsyncFunc(AsyncFuncScheduler& func_scheduler,
                     ITask& task,
                     TickType_t wait_interval)
    : wait_interval_(wait_interval)
    , func_scheduler_(func_scheduler)
    , task_(task) {
    configASSERT(wait_interval_);
}

status::StatusCode AsyncFunc::run() {
    auto future = func_scheduler_.add([this]() {
        return task_.run();
    });
    if (!future) {
        return status::StatusCode::InvalidState;
    }

    const auto code = future->wait(wait_interval_);
    if (code != status::StatusCode::OK) {
        return code;
    }

    return future->code();
}

} // namespace scheduler
} // namespace ocs
