/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_pipeline/httpserver/reboot_handler.h"
#include "ocs_algo/response_ops.h"
#include "ocs_core/log.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

namespace {

const char* log_tag = "http_reboot_handler";

} // namespace

RebootHandler::RebootHandler(scheduler::ITask& reboot_task)
    : reboot_task_(reboot_task) {
}

status::StatusCode RebootHandler::serve_http(http::IResponseWriter& w, http::IRequest&) {
    const auto code = algo::ResponseOps::write_text(w, "Rebooting...");
    if (code != status::StatusCode::OK) {
        return code;
    }

    ocs_logi(log_tag, "Rebooting...");

    return reboot_task_.run();
}

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
