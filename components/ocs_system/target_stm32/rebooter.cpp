/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_system/target_stm32/rebooter.h"
#include "ocs_core/freertos.h"
#include "ocs_core/log.h"
#include "ocs_system/target_stm32/stm32hal.h"

namespace ocs {
namespace system {

namespace {

const char* log_tag = "rebooter";

} // namespace

Rebooter::Rebooter(IRebootHandler& handler, TickType_t delay)
    : delay_(delay)
    , handler_(handler) {
}

status::StatusCode Rebooter::reboot() {
    ocs_logi(log_tag, "Prepare for rebooting...");
    handler_.handle_reboot();
    ocs_logi(log_tag, "Ready for rebooting...");

    if (delay_) {
        vTaskDelay(delay_);
    }

    NVIC_SystemReset();

    return status::StatusCode::OK;
}

} // namespace system
} // namespace ocs
