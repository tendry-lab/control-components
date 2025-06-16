/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_net/wait_network_runner.h"
#include "ocs_core/log.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace net {

namespace {

const char* log_tag = "wait_network_runner";

} // namespace

WaitNetworkRunner::WaitNetworkRunner(INetwork& network, TickType_t wait)
    : wait_start_interval_(wait)
    , network_(network) {
}

status::StatusCode WaitNetworkRunner::start() {
    const auto code = start_();
    if (code != status::StatusCode::OK) {
        stop();
    }

    return code;
}

status::StatusCode WaitNetworkRunner::stop() {
    status::StatusCode code = status::StatusCode::OK;

    if (!stopped_) {
        code = network_.stop();
        if (code != status::StatusCode::OK) {
            ocs_loge(log_tag, "failed to stop network: %s", status::code_to_str(code));
        }

        stopped_ = true;
    }

    return code;
}

status::StatusCode WaitNetworkRunner::start_() {
    auto code = network_.start();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to start network: %s", status::code_to_str(code));

        return code;
    }

    code = network_.wait(wait_start_interval_);
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to wait for network: %s", status::code_to_str(code));

        return code;
    }

    return status::StatusCode::OK;
}

} // namespace net
} // namespace ocs
