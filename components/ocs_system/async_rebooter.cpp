/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_system/async_rebooter.h"

namespace ocs {
namespace system {

AsyncRebooter::AsyncRebooter(EventGroupHandle_t handle, EventBits_t event)
    : event_(event)
    , handle_(handle) {
}

status::StatusCode AsyncRebooter::reboot() {
    xEventGroupSetBits(handle_, event_);
    return status::StatusCode::OK;
}

} // namespace system
} // namespace ocs
