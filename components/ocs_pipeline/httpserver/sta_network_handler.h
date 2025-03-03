/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_algo/uri_ops.h"
#include "ocs_core/noncopyable.h"
#include "ocs_core/static_mutex.h"
#include "ocs_http/iserver.h"
#include "ocs_net/sta_network_config.h"
#include "ocs_scheduler/itask.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

class StaNetworkHandler : public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p server to register HTTP endpoints.
    //!  - @p config to perform the WiFi STA configuration.
    //!  - @p reboot_task to schedule a reboot when the configuration is changed.
    StaNetworkHandler(http::IServer& server,
                      net::StaNetworkConfig& config,
                      scheduler::ITask& reboot_task);

private:
    status::StatusCode handle_update_(http::IResponseWriter& w,
                                      const algo::UriOps::Values&);

    status::StatusCode handle_get_(http::IResponseWriter& w);

    core::StaticMutex mu_;
    net::StaNetworkConfig& config_;
    scheduler::ITask& reboot_task_;
};

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
