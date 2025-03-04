/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_algo/response_ops.h"
#include "ocs_core/log.h"
#include "ocs_pipeline/httpserver/system_handler.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

namespace {

const char* log_tag = "http_system_handler";

} // namespace

SystemHandler::SystemHandler(http::IRouter& router, scheduler::ITask& reboot_task) {
    router.add(http::IRouter::Method::Get, "/api/v1/system/reboot",
               [&reboot_task](http::IResponseWriter& w, http::IRequest&) {
                   const auto code = algo::ResponseOps::write_text(w, "Rebooting...");
                   if (code != status::StatusCode::OK) {
                       return code;
                   }

                   ocs_logi(log_tag, "Rebooting...");

                   return reboot_task.run();
               });
}

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
