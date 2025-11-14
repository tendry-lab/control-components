/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_pipeline/httpserver/http_pipeline.h"
#include "ocs_pipeline/httpserver/data_handler.h"
#include "ocs_pipeline/httpserver/mdns_handler.h"
#include "ocs_pipeline/httpserver/reboot_handler.h"
#include "ocs_status/code_to_str.h"

#ifdef CONFIG_FREERTOS_USE_TRACE_FACILITY
#include "ocs_pipeline/httpserver/system_state_handler.h"
#endif // CONFIG_FREERTOS_USE_TRACE_FACILITY

namespace ocs {
namespace pipeline {
namespace httpserver {

HttpPipeline::HttpPipeline(scheduler::ITask& reboot_task,
                           net::MdnsConfig& mdns_config,
                           http::IRouter& router,
                           fmt::json::IFormatter& telemetry_formatter,
                           fmt::json::FanoutFormatter& registration_formatter,
                           Params params) {
    telemetry_handler_.reset(new (std::nothrow) DataHandler(
        telemetry_formatter, params.telemetry.buffer_size));
    configASSERT(telemetry_handler_);

    registration_handler_.reset(new (std::nothrow) DataHandler(
        registration_formatter, params.registration.buffer_size));
    configASSERT(registration_handler_);

    reboot_handler_.reset(new (std::nothrow) RebootHandler(reboot_task));
    configASSERT(reboot_handler_);

    mdns_handler_.reset(new (std::nothrow) MdnsHandler(mdns_config, reboot_task));
    configASSERT(mdns_handler_);

#ifdef CONFIG_FREERTOS_USE_TRACE_FACILITY
    system_state_handler_.reset(new (std::nothrow) SystemStateHandler(1024 * 2));
    configASSERT(system_state_handler_);

    router.add(http::IRouter::Method::Get, "/api/v1/system/report",
               *system_state_handler_);
#endif // CONFIG_FREERTOS_USE_TRACE_FACILITY
}

http::IHandler& HttpPipeline::get_registration_handler() {
    return *registration_handler_;
}

http::IHandler& HttpPipeline::get_telemetry_handler() {
    return *telemetry_handler_;
}

http::IHandler& HttpPipeline::get_mdns_handler() {
    return *mdns_handler_;
}

http::IHandler& HttpPipeline::get_reboot_handler() {
    return *reboot_handler_;
}

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
