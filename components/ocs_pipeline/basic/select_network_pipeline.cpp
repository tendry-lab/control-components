/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_pipeline/basic/select_network_pipeline.h"
#include "ocs_core/freertos.h"
#include "ocs_net/reset_network_runner.h"
#include "ocs_net/wait_network_runner.h"

namespace ocs {
namespace pipeline {
namespace basic {

SelectNetworkPipeline::SelectNetworkPipeline(storage::IStorage& sta_config_storage,
                                             storage::IStorage& ap_config_storage,
                                             net::INetworkHandler& network_handler,
                                             control::ConfigFsrHandler& fsr_handler,
                                             system::IArena& arena,
                                             system::IRebooter& rebooter,
                                             const system::DeviceInfo& device_info) {
    sta_config_ =
        ocs::system::make_unique_ptr<net::StaNetworkConfig>(arena, sta_config_storage);
    configASSERT(sta_config_);

    net::INetwork* network = nullptr;
    storage::IConfig* network_config = nullptr;

    if (sta_config_->valid()) {
        sta_ = ocs::system::make_unique_ptr<net::StaNetwork>(arena, network_handler,
                                                             *sta_config_);
        configASSERT(sta_);

        network = sta_.get();
        network_config = sta_config_.get();
    } else {
        ap_config_ = ocs::system::make_unique_ptr<net::ApNetworkConfig>(
            arena, ap_config_storage, device_info);
        configASSERT(ap_config_);

        ap_ = ocs::system::make_unique_ptr<net::ApNetwork>(arena, network_handler,
                                                           *ap_config_);
        configASSERT(ap_);

        network = ap_.get();
        network_config = ap_config_.get();
    }

    configASSERT(network);
    configASSERT(network_config);

    wait_runner_ = ocs::system::make_unique_ptr<net::WaitNetworkRunner>(
        arena, *network, wait_start_interval_);
    configASSERT(wait_runner_);

    runner_ = wait_runner_.get();

    reset_runner_ = ocs::system::make_unique_ptr<net::ResetNetworkRunner>(
        arena, *runner_, *network_config, rebooter);
    configASSERT(reset_runner_);

    runner_ = reset_runner_.get();
    configASSERT(runner_);

    if (auto config = get_ap_config(); config) {
        fsr_handler.add(*config);
    }
    fsr_handler.add(get_sta_config());
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
