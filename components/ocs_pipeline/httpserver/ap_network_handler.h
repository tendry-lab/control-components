/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_algo/uri_ops.h"
#include "ocs_core/noncopyable.h"
#include "ocs_core/static_mutex.h"
#include "ocs_http/ihandler.h"
#include "ocs_http/irouter.h"
#include "ocs_net/ap_network_config.h"
#include "ocs_scheduler/itask.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

class ApNetworkHandler : private http::IHandler, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p router to register HTTP endpoints.
    //!  - @p config to perform the WiFi AP configuration.
    //!  - @p reboot_task to schedule a reboot when the configuration is changed.
    ApNetworkHandler(http::IRouter& router,
                     net::ApNetworkConfig& config,
                     scheduler::ITask& reboot_task);

private:
    status::StatusCode serve_http(http::IResponseWriter& w, http::IRequest&) override;

    status::StatusCode handle_update_(http::IResponseWriter& w,
                                      const algo::UriOps::Values&);

    status::StatusCode handle_get_(http::IResponseWriter& w);

    core::StaticMutex mu_;
    net::ApNetworkConfig& config_;
    scheduler::ITask& reboot_task_;
};

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
