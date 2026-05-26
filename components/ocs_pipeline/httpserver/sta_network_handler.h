/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_algo/uri_ops.h"
#include "ocs_core/noncopyable.h"
#include "ocs_core/static_mutex.h"
#include "ocs_http/ihandler.h"
#include "ocs_http/irequest.h"
#include "ocs_http/iresponse_writer.h"
#include "ocs_net/sta_network_config.h"
#include "ocs_system/iarena.h"
#include "ocs_system/irebooter.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

class StaNetworkHandler : public http::IHandler, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p arena to perform dynamic allocations.
    //!  - @p rebooter to reboot the system when the configuration is changed.
    //!  - @p config to perform the WiFi STA configuration.
    StaNetworkHandler(system::IArena& arena,
                      system::IRebooter& rebooter,
                      net::StaNetworkConfig& config);

    // Update WiFi STA network configuration over HTTP.
    status::StatusCode serve_http(http::IResponseWriter& w, http::IRequest&) override;

private:
    status::StatusCode handle_update_(http::IResponseWriter& w,
                                      const algo::UriOps::Values&);

    status::StatusCode handle_get_(http::IResponseWriter& w);

    core::StaticMutex mu_;
    system::IArena& arena_;
    system::IRebooter& rebooter_;
    net::StaNetworkConfig& config_;
};

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
