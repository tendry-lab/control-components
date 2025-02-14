/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_net/reset_network_runner.h"
#include "ocs_core/log.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace net {

namespace {

const char* log_tag = "reset_network_runner";

} // namespace

ResetNetworkRunner::ResetNetworkRunner(INetworkRunner& runner,
                                       INetworkConfig& config,
                                       system::IRebooter& rebooter)
    : runner_(runner)
    , config_(config)
    , rebooter_(rebooter) {
}

status::StatusCode ResetNetworkRunner::start() {
    const auto code = runner_.start();
    if (code != status::StatusCode::OK) {
        reset_();
    }

    return code;
}

status::StatusCode ResetNetworkRunner::stop() {
    return runner_.stop();
}

void ResetNetworkRunner::reset_() {
    ocs_logi(log_tag, "resetting network configuration");

    auto code = config_.reset();
    if (code == status::StatusCode::NotModified) {
        code = status::StatusCode::OK;
    }

    if (code != status::StatusCode::OK) {
        ocs_logw(log_tag, "failed to reset network configuration: %s",
                 status::code_to_str(code));
    }

    rebooter_.reboot();
}

} // namespace net
} // namespace ocs
