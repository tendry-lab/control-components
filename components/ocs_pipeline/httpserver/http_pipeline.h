/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/fanout_formatter.h"
#include "ocs_http/irouter.h"
#include "ocs_net/mdns_config.h"
#include "ocs_scheduler/itask.h"
#include "ocs_system/iarena.h"
#include "ocs_system/irebooter.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

class HttpPipeline : private core::NonCopyable<> {
public:
    struct DataParams {
        //! Buffer size to hold the formatted JSON data, in bytes.
        size_t buffer_size { 0 };
    };

    struct Params {
        DataParams telemetry;
        DataParams registration;
    };

    //! Initialize.
    HttpPipeline(system::IArena& arena,
                 system::IRebooter& rebooter,
                 net::MdnsConfig& mdns_config,
                 http::IRouter& router,
                 fmt::json::IFormatter& telemetry_formatter,
                 fmt::json::FanoutFormatter& registration_formatter,
                 Params params);

    http::IHandler& get_registration_handler();
    http::IHandler& get_telemetry_handler();
    http::IHandler& get_mdns_handler();
    http::IHandler& get_reboot_handler();

private:
    system::UniquePtr<http::IHandler> telemetry_handler_;
    system::UniquePtr<http::IHandler> registration_handler_;
    system::UniquePtr<http::IHandler> reboot_handler_;
    system::UniquePtr<http::IHandler> mdns_handler_;

#ifdef CONFIG_FREERTOS_USE_TRACE_FACILITY
    system::UniquePtr<http::IHandler> system_state_handler_;
#endif // CONFIG_FREERTOS_USE_TRACE_FACILITY
};

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
