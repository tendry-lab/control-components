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
                           system::FanoutSuspender& suspender,
                           net::FanoutNetworkHandler& network_handler,
                           net::IMdnsDriver& mdns_driver,
                           fmt::json::IFormatter& telemetry_formatter,
                           fmt::json::FanoutFormatter& registration_formatter,
                           config::MdnsConfig& mdns_config,
                           Params params)
    : mdns_driver_(mdns_driver) {
    mdns_driver.add_service(net::IMdnsDriver::Service::Http, net::IMdnsDriver::Proto::Tcp,
                            CONFIG_OCS_HTTP_SERVER_PORT);

    const char* api_base_path = "/api/v1";

    configASSERT(mdns_driver.add_txt_record(net::IMdnsDriver::Service::Http,
                                            net::IMdnsDriver::Proto::Tcp, "api",
                                            api_base_path)
                 == status::StatusCode::OK);

    const auto autodiscovery_uri = std::string("http://") + mdns_driver.get_dns_name()
        + ":" + std::to_string(CONFIG_OCS_HTTP_SERVER_PORT) + api_base_path;

    configASSERT(mdns_driver.add_txt_record(
                     net::IMdnsDriver::Service::Http, net::IMdnsDriver::Proto::Tcp,
                     "autodiscovery_uri", autodiscovery_uri.c_str())
                 == status::StatusCode::OK);

    configASSERT(mdns_driver.add_txt_record(
                     net::IMdnsDriver::Service::Http, net::IMdnsDriver::Proto::Tcp,
                     "autodiscovery_desc", mdns_config.get_instance_name())
                 == status::StatusCode::OK);

    configASSERT(mdns_driver.add_txt_record(net::IMdnsDriver::Service::Http,
                                            net::IMdnsDriver::Proto::Tcp,
                                            "autodiscovery_mode", "1")
                 == status::StatusCode::OK);

    network_handler.add(*this);

    http_server_.reset(new (std::nothrow) http::Server(params.server));
    configASSERT(http_server_);

    configASSERT(suspender.add(*this, "http_pipeline") == status::StatusCode::OK);

    telemetry_handler_.reset(new (std::nothrow) DataHandler(
        *http_server_, telemetry_formatter, "/api/v1/telemetry", "http_telemetry_handler",
        params.telemetry.buffer_size));
    configASSERT(telemetry_handler_);

    registration_handler_.reset(new (std::nothrow) DataHandler(
        *http_server_, registration_formatter, "/api/v1/registration",
        "http_registration_handler", params.registration.buffer_size));
    configASSERT(registration_handler_);

    system_handler_.reset(new (std::nothrow) SystemHandler(*http_server_, reboot_task));
    configASSERT(system_handler_);

    mdns_handler_.reset(new (std::nothrow)
                            MdnsHandler(*http_server_, mdns_config, reboot_task));
    configASSERT(mdns_handler_);

#ifdef CONFIG_FREERTOS_USE_TRACE_FACILITY
    system_state_handler_.reset(new (std::nothrow)
                                    SystemStateHandler(*http_server_, 1024 * 2));
    configASSERT(system_state_handler_);
#endif // CONFIG_FREERTOS_USE_TRACE_FACILITY
}

void HttpPipeline::handle_connect() {
    const auto code = http_server_->start();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to start HTTP server on network connect: code=%s",
                 status::code_to_str(code));
    }
}

void HttpPipeline::handle_disconnect() {
    const auto code = http_server_->stop();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag,
                 "failed to stop HTTP server when on network disconnect: code=%s",
                 status::code_to_str(code));
    }
}

status::StatusCode HttpPipeline::handle_suspend() {
    return mdns_driver_.stop();
}

status::StatusCode HttpPipeline::handle_resume() {
    return mdns_driver_.start();
}

http::Server& HttpPipeline::get_server() {
    return *http_server_;
}

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
