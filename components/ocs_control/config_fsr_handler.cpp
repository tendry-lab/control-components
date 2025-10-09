/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
