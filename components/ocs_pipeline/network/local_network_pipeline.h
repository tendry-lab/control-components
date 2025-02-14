/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>

#include "ocs_core/noncopyable.h"
#include "ocs_net/ap_network.h"
#include "ocs_net/inetwork_handler.h"
#include "ocs_pipeline/config/ap_network_config.h"

namespace ocs {
namespace pipeline {
namespace network {

//! Local WiFi AP.
class LocalNetworkPipeline : public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p handler to notify about network connection status.
    //!  - @p config to read network settings.
    LocalNetworkPipeline(net::INetworkHandler& handler,
                         const config::ApNetworkConfig& config);

    net::IApNetwork& get_network();

    //! Start the WiFi network.
    status::StatusCode start();

private:
    status::StatusCode start_();

    static const TickType_t wait_start_interval_ = pdMS_TO_TICKS(1000 * 60 * 10);

    std::unique_ptr<net::ApNetwork> network_;
};

} // namespace network
} // namespace pipeline
} // namespace ocs
