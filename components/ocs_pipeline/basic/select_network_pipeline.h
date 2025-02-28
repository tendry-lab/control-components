/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>

#include "ocs_core/noncopyable.h"
#include "ocs_net/ap_network_config.h"
#include "ocs_net/inetwork_handler.h"
#include "ocs_net/inetwork_runner.h"
#include "ocs_net/sta_network_config.h"
#include "ocs_net/target_esp32/ap_network.h"
#include "ocs_net/target_esp32/sta_network.h"
#include "ocs_storage/storage_builder.h"
#include "ocs_system/device_info.h"
#include "ocs_system/irebooter.h"

namespace ocs {
namespace pipeline {
namespace basic {

// Select between WiFi AP and STA networks.
class SelectNetworkPipeline : public core::NonCopyable<> {
public:
    //! Initialize.
    SelectNetworkPipeline(storage::StorageBuilder& storage_builder,
                          net::INetworkHandler& network_handler,
                          system::IRebooter& rebooter,
                          const system::DeviceInfo& device_info);

    net::INetworkRunner& get_runner();
    net::IApNetwork* get_ap_network();
    net::IStaNetwork* get_sta_network();
    net::ApNetworkConfig* get_ap_config();
    net::StaNetworkConfig& get_sta_config();

private:
    static constexpr const char* ap_config_storage_id_ = "wifi_ap_cfg";
    static constexpr const char* sta_config_storage_id_ = "wifi_sta_cfg";

    static constexpr TickType_t wait_start_interval_ = pdMS_TO_TICKS(1000 * 60 * 10);

    storage::StorageBuilder::IStoragePtr sta_config_storage_;
    std::unique_ptr<net::StaNetworkConfig> sta_config_;
    std::unique_ptr<net::StaNetwork> sta_;

    storage::StorageBuilder::IStoragePtr ap_config_storage_;
    std::unique_ptr<net::ApNetworkConfig> ap_config_;
    std::unique_ptr<net::ApNetwork> ap_;

    std::unique_ptr<net::INetworkRunner> wait_runner_;
    std::unique_ptr<net::INetworkRunner> reset_runner_;
    net::INetworkRunner* runner_ { nullptr };
};

} // namespace basic
} // namespace pipeline
} // namespace ocs
