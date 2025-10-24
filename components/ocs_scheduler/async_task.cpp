/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_scheduler/async_task.h"

namespace ocs {
namespace scheduler {

AsyncTask::AsyncTask(EventGroupHandle_t handle, EventBits_t event)
    : event_(event)
    , handle_(handle) {
}

status::StatusCode AsyncTask::run() {
    xEventGroupSetBits(handle_, event_);
    return status::StatusCode::OK;
}

} // namespace scheduler
} // namespace ocs
