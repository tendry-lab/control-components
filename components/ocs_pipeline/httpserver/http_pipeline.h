/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/fanout_formatter.h"
#include "ocs_http/irouter.h"
#include "ocs_http/iserver.h"
#include "ocs_net/fanout_network_handler.h"
#include "ocs_pipeline/httpserver/mdns_handler.h"
#include "ocs_pipeline/httpserver/system_handler.h"
#include "ocs_scheduler/itask.h"

#ifdef CONFIG_FREERTOS_USE_TRACE_FACILITY
#include "ocs_pipeline/httpserver/system_state_handler.h"
#endif // CONFIG_FREERTOS_USE_TRACE_FACILITY

namespace ocs {
namespace pipeline {
namespace httpserver {

class HttpPipeline : public net::INetworkHandler, private core::NonCopyable<> {
public:
    struct DataParams {
        //! Buffer size to hold the formatted JSON data, in bytes.
        unsigned buffer_size { 0 };
    };

    struct Params {
        DataParams telemetry;
        DataParams registration;
    };

    //! Initialize.
    HttpPipeline(scheduler::ITask& reboot_task,
                 net::FanoutNetworkHandler& network_handler,
                 net::MdnsConfig& mdns_config,
                 http::IServer& server,
                 http::IRouter& router,
                 fmt::json::IFormatter& telemetry_formatter,
                 fmt::json::FanoutFormatter& registration_formatter,
                 Params params);

    //! Start HTTP server.
    void handle_connect() override;

    //! Stop HTTP server.
    void handle_disconnect() override;

    http::IHandler& get_registration_handler();
    http::IHandler& get_telemetry_handler();

private:
    http::IServer& server_;

    std::unique_ptr<http::IHandler> telemetry_handler_;
    std::unique_ptr<http::IHandler> registration_handler_;
    std::unique_ptr<SystemHandler> system_handler_;
    std::unique_ptr<MdnsHandler> mdns_handler_;

#ifdef CONFIG_FREERTOS_USE_TRACE_FACILITY
    std::unique_ptr<SystemStateHandler> system_state_handler_;
#endif // CONFIG_FREERTOS_USE_TRACE_FACILITY
};

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
