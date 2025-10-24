/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
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
                                       storage::IConfig& config,
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
