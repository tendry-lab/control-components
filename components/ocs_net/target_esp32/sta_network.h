/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "esp_wifi.h"

#include "ocs_core/noncopyable.h"
#include "ocs_core/static_event_group.h"
#include "ocs_net/inetwork.h"
#include "ocs_net/inetwork_handler.h"
#include "ocs_net/ista_network.h"
#include "ocs_net/sta_network_config.h"
#include "ocs_net/target_esp32/netif_builder.h"
#include "ocs_status/code.h"

namespace ocs {
namespace net {

//! Handle WiFi STA (station) network operations.
class StaNetwork : public INetwork, public IStaNetwork, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p handler to notify about network status changes.
    StaNetwork(INetworkHandler& handler, const StaNetworkConfig& config);

    //! Destroy.
    ~StaNetwork();

    //! Return various network characteristics.
    IStaNetwork::Info get_info() override;

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

    ip_addr_t get_ip_addr_() const;

    void handle_wifi_event_(int32_t event_id, void* event_data);
    void handle_wifi_event_sta_start_();
    void handle_wifi_event_sta_disconnected_(void* event_data);

    void handle_ip_event_(int32_t event_id, void* event_data);
    void handle_ip_event_sta_got_ip_(void* event_data);

    const StaNetworkConfig& config_;

    INetworkHandler& handler_;

    NetifSharedPtr netif_;

    core::StaticEventGroup event_group_;

    esp_event_handler_instance_t instance_any_id_ { nullptr };
    esp_event_handler_instance_t instance_got_ip_ { nullptr };

    unsigned retry_count_ { 0 };
};

} // namespace net
} // namespace ocs
