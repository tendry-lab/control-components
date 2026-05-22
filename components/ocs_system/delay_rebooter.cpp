/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_system/delay_rebooter.h"

namespace ocs {
namespace system {

DelayRebooter::DelayRebooter(IRebooter& rebooter, TickType_t delay)
    : delay_(delay)
    , rebooter_(rebooter) {
}

status::StatusCode DelayRebooter::reboot() {
    vTaskDelay(delay_);
    return rebooter_.reboot();
}

} // namespace system
} // namespace ocs
