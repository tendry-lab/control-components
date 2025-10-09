/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_net/reset_network_runner.h"
#include "ocs_net/wait_network_runner.h"
#include "ocs_pipeline/basic/select_network_pipeline.h"

namespace ocs {
namespace pipeline {
namespace basic {

SelectNetworkPipeline::SelectNetworkPipeline(storage::StorageBuilder& storage_builder,
                                             net::INetworkHandler& network_handler,
                                             system::IRebooter& rebooter,
                                             const system::DeviceInfo& device_info) {
    sta_config_storage_ = storage_builder.make(sta_config_storage_id_);
    configASSERT(sta_config_storage_);

    sta_config_.reset(new (std::nothrow) net::StaNetworkConfig(*sta_config_storage_));
    configASSERT(sta_config_);

    net::INetwork* network = nullptr;
    storage::IConfig* network_config = nullptr;

    if (sta_config_->valid()) {
        sta_.reset(new (std::nothrow) net::StaNetwork(network_handler, *sta_config_));
        configASSERT(sta_);

        network = sta_.get();
        network_config = sta_config_.get();
    } else {
        ap_config_storage_ = storage_builder.make(ap_config_storage_id_);
        configASSERT(ap_config_storage_);

        ap_config_.reset(new (std::nothrow)
                             net::ApNetworkConfig(*ap_config_storage_, device_info));
        configASSERT(ap_config_);

        ap_.reset(new (std::nothrow) net::ApNetwork(network_handler, *ap_config_));
        configASSERT(ap_);

        network = ap_.get();
        network_config = ap_config_.get();
    }

    configASSERT(network);
    configASSERT(network_config);

    wait_runner_.reset(new (std::nothrow)
                           net::WaitNetworkRunner(*network, wait_start_interval_));
    configASSERT(wait_runner_);

    runner_ = wait_runner_.get();

    reset_runner_.reset(new (std::nothrow)
                            net::ResetNetworkRunner(*runner_, *network_config, rebooter));
    configASSERT(reset_runner_);

    runner_ = reset_runner_.get();
    configASSERT(runner_);
}

net::INetworkRunner& SelectNetworkPipeline::get_runner() {
    return *runner_;
}

net::IApNetwork* SelectNetworkPipeline::get_ap_network() {
    return ap_ ? ap_.get() : nullptr;
}

net::IStaNetwork* SelectNetworkPipeline::get_sta_network() {
    return sta_ ? sta_.get() : nullptr;
}

net::ApNetworkConfig* SelectNetworkPipeline::get_ap_config() {
    return ap_config_ ? ap_config_.get() : nullptr;
}

net::StaNetworkConfig& SelectNetworkPipeline::get_sta_config() {
    return *sta_config_;
}

} // namespace basic
} // namespace pipeline
} // namespace ocs
