/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_system/freertos_timer.h"
#include "ocs_core/freertos.h"
#include "ocs_core/log.h"

namespace ocs {
namespace system {

namespace {

const char* log_tag = "freertos_timer";

} // namespace

FreeRtosTimer::FreeRtosTimer(scheduler::ITask& task,
                             const char* id,
                             system::Time interval)
    : task_(task) {
    configASSERT(id);
    configASSERT(interval > 0);

    const TickType_t period = pdMS_TO_TICKS(interval / Duration::millisecond);
    configASSERT(period > 0);

    handle_ = xTimerCreate(id, period, pdTRUE, this, handle_cb_);
    configASSERT(handle_);
}

FreeRtosTimer::~FreeRtosTimer() {
    configASSERT(xTimerDelete(handle_, portMAX_DELAY) == pdPASS);
}

status::StatusCode FreeRtosTimer::start() {
    if (xTimerStart(handle_, portMAX_DELAY) != pdPASS) {
        ocs_loge(log_tag, "xTimerStart() failed");
        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

status::StatusCode FreeRtosTimer::stop() {
    if (xTimerStop(handle_, portMAX_DELAY) != pdPASS) {
        ocs_loge(log_tag, "xTimerStop() failed");
        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

void FreeRtosTimer::handle_cb_(TimerHandle_t handle) {
    auto& self = *static_cast<FreeRtosTimer*>(pvTimerGetTimerID(handle));
    self.task_.run();
}

} // namespace system
} // namespace ocs
