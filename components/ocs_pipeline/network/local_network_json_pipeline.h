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
#include "ocs_fmt/json/fanout_formatter.h"
#include "ocs_pipeline/config/ap_network_config.h"
#include "ocs_pipeline/network/local_network_pipeline.h"
#include "ocs_status/code.h"
#include "ocs_system/device_info.h"

namespace ocs {
namespace pipeline {
namespace network {

class LocalNetworkJsonPipeline : public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p handler to notify about network connection status.
    //!  - @p registraton_formatter to format network characteristics.
    //!  - @p config to read network settings.
    LocalNetworkJsonPipeline(net::INetworkHandler& handler,
                             fmt::json::FanoutFormatter& registraton_formatter,
                             const config::ApNetworkConfig& config);

    LocalNetworkPipeline& get_network_pipeline();

private:
    std::unique_ptr<LocalNetworkPipeline> network_pipeline_;
    std::unique_ptr<fmt::json::IFormatter> network_formatter_;
};

} // namespace network
} // namespace pipeline
} // namespace ocs
