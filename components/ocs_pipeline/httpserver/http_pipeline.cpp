/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_pipeline/httpserver/http_pipeline.h"
#include "ocs_core/log.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

namespace {

const char* log_tag = "http_pipeline";

} // namespace

HttpPipeline::HttpPipeline(scheduler::ITask& reboot_task,
                           net::FanoutNetworkHandler& network_handler,
                           net::MdnsConfig& mdns_config,
                           http::IServer& server,
                           http::IRouter& router,
                           fmt::json::IFormatter& telemetry_formatter,
                           fmt::json::FanoutFormatter& registration_formatter,
                           Params params)
    : server_(server) {
    network_handler.add(*this);

    telemetry_handler_.reset(new (std::nothrow) DataHandler(
        router, telemetry_formatter, "/api/v1/telemetry", "http_telemetry_handler",
        params.telemetry.buffer_size));
    configASSERT(telemetry_handler_);

    registration_handler_.reset(new (std::nothrow) DataHandler(
        router, registration_formatter, "/api/v1/registration",
        "http_registration_handler", params.registration.buffer_size));
    configASSERT(registration_handler_);

    system_handler_.reset(new (std::nothrow) SystemHandler(router, reboot_task));
    configASSERT(system_handler_);

    mdns_handler_.reset(new (std::nothrow) MdnsHandler(router, mdns_config, reboot_task));
    configASSERT(mdns_handler_);

#ifdef CONFIG_FREERTOS_USE_TRACE_FACILITY
    system_state_handler_.reset(new (std::nothrow) SystemStateHandler(router, 1024 * 2));
    configASSERT(system_state_handler_);
#endif // CONFIG_FREERTOS_USE_TRACE_FACILITY
}

void HttpPipeline::handle_connect() {
    const auto code = server_.start();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to start HTTP server on network connect: code=%s",
                 status::code_to_str(code));
    }
}

void HttpPipeline::handle_disconnect() {
    const auto code = server_.stop();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag,
                 "failed to stop HTTP server when on network disconnect: code=%s",
                 status::code_to_str(code));
    }
}

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
