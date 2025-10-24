/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_system/fanout_reboot_handler.h"

namespace ocs {
namespace system {

void FanoutRebootHandler::handle_reboot() {
    for (auto& handler : handlers_) {
        handler->handle_reboot();
    }
}

void FanoutRebootHandler::add(IRebootHandler& handler) {
    handlers_.emplace_back(&handler);
}

} // namespace system
} // namespace ocs
