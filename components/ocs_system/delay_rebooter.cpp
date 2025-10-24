/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_system/delay_rebooter.h"

namespace ocs {
namespace system {

DelayRebooter::DelayRebooter(TickType_t delay, IRebooter& rebooter)
    : delay_(delay)
    , rebooter_(rebooter) {
}

void DelayRebooter::reboot() {
    vTaskDelay(delay_);
    rebooter_.reboot();
}

} // namespace system
} // namespace ocs
