/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_control/config_fsr_handler.h"

namespace ocs {
namespace control {

status::StatusCode ConfigFsrHandler::handle_fsr() {
    for (auto& config : configs_) {
        config->reset();
    }

    return status::StatusCode::OK;
}

void ConfigFsrHandler::add(storage::IConfig& config) {
    configs_.push_back(&config);
}

} // namespace control
} // namespace ocs
