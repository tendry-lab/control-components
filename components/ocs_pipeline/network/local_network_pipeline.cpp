/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>
#include <string>

#include "ocs_algo/storage_ops.h"
#include "ocs_core/log.h"
#include "ocs_net/ap_network.h"
#include "ocs_net/sta_network.h"
#include "ocs_pipeline/network/local_network_pipeline.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace pipeline {
namespace network {

namespace {

const char* log_tag = "local_network_pipeline";

} // namespace

LocalNetworkPipeline::LocalNetworkPipeline(net::INetworkHandler& handler,
                                           const config::ApNetworkConfig& config) {
    network_.reset(new (std::nothrow)
                       net::ApNetwork(handler,
                                      net::ApNetwork::Params {
                                          .ssid = config.get_ssid(),
                                          .password = config.get_password(),
                                          .channel = config.get_channel(),
                                          .max_connection = config.get_max_conn(),
                                      }));
}

net::IApNetwork& LocalNetworkPipeline::get_network() {
    return *network_;
}

status::StatusCode LocalNetworkPipeline::start() {
    auto code = start_();
    if (code != status::StatusCode::OK) {
        code = network_->stop();
        if (code != status::StatusCode::OK) {
            ocs_loge(log_tag, "failed to stop network: %s", status::code_to_str(code));
        }
    }

    return code;
}

status::StatusCode LocalNetworkPipeline::start_() {
    auto code = network_->start();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to start network: %s", status::code_to_str(code));

        return code;
    }

    code = network_->wait(wait_start_interval_);
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to wait for network: %s", status::code_to_str(code));

        return code;
    }

    return status::StatusCode::OK;
}

} // namespace network
} // namespace pipeline
} // namespace ocs
