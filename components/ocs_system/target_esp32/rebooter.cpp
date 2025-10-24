/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_system.h"

#include "ocs_core/log.h"
#include "ocs_system/target_esp32/rebooter.h"

namespace ocs {
namespace system {

namespace {

const char* log_tag = "rebooter";

} // namespace

Rebooter::Rebooter(IRebootHandler& handler)
    : handler_(handler) {
}

void Rebooter::reboot() {
    ocs_logi(log_tag, "Prepare for rebooting...");
    handler_.handle_reboot();
    ocs_logi(log_tag, "Ready for rebooting...");

    return esp_restart();
}

} // namespace system
} // namespace ocs
