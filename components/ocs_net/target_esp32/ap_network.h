/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_wifi.h"

#include "ocs_core/cond.h"
#include "ocs_core/noncopyable.h"
#include "ocs_core/static_event_group.h"
#include "ocs_core/static_mutex.h"
#include "ocs_net/ap_network_config.h"
#include "ocs_net/iap_network.h"
#include "ocs_net/inetwork.h"
#include "ocs_net/inetwork_handler.h"
#include "ocs_net/target_esp32/netif_builder.h"
#include "ocs_status/code.h"

namespace ocs {
namespace net {

//! Handle WiFi AP (access-point) mode network operations.
class ApNetwork : public INetwork, public IApNetwork, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p handler to notify about network status changes.
    ApNetwork(INetworkHandler& handler, const ApNetworkConfig& config);

    //! Destroy.
    ~ApNetwork();

    //! Return various network characteristics.
    IApNetwork::Info get_info() override;

    //! Start the WiFi connection process.
    status::StatusCode start() override;

    //! Stop the WiFi connection process.
    status::StatusCode stop() override;

    //! Wait for the WiFi connection to be established.
    status::StatusCode wait(TickType_t wait = portMAX_DELAY) override;

private:
    static void handle_event_(void* event_arg,
                              esp_event_base_t event_base,
                              int32_t event_id,
                              void* event_data);

    void handle_wifi_event_(int32_t event_id, void* event_data);
    void handle_wifi_event_ap_sta_start_();
    void handle_wifi_event_ap_sta_stop_();
    void handle_wifi_event_ap_sta_connected_(void* event_data);
    void handle_wifi_event_ap_sta_disconnected_(void* event_data);

    const ApNetworkConfig& config_;

    INetworkHandler& handler_;

    NetifSharedPtr netif_;

    esp_event_handler_instance_t instance_any_id_ { nullptr };

    core::StaticMutex mu_;
    core::Cond cond_;
    status::StatusCode code_ { status::StatusCode::Last };
    uint8_t connection_count_ { 0 };
};

} // namespace net
} // namespace ocs
