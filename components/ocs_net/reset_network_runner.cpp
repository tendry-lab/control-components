/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_net/reset_network_runner.h"
#include "ocs_core/log.h"
#include "ocs_status/code_to_str.h"
#include "ocs_status/macros.h"

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
    const auto start_code = runner_.start();
    const auto reset_code = reset_();

    OCS_STATUS_RETURN_ON_ERROR(start_code);
    OCS_STATUS_RETURN_ON_ERROR(reset_code);

    return status::StatusCode::OK;
}

status::StatusCode ResetNetworkRunner::stop() {
    return runner_.stop();
}

status::StatusCode ResetNetworkRunner::reset_() {
    ocs_logi(log_tag, "resetting network configuration");

    auto code = config_.reset();
    if (code == status::StatusCode::NotModified) {
        code = status::StatusCode::OK;
    }

    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to reset network config: %s",
                 status::code_to_str(code));

        return code;
    }

    code = rebooter_.reboot();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to reboot on config reset: %s",
                 status::code_to_str(code));

        return code;
    }

    return status::StatusCode::OK;
}

} // namespace net
} // namespace ocs
