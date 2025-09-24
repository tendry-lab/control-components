/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_algo/response_ops.h"
#include "ocs_fmt/json/cjson_builder.h"
#include "ocs_pipeline/httpserver/system_state_handler.h"
#include "ocs_pipeline/jsonfmt/system_state_formatter.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

SystemStateHandler::SystemStateHandler(unsigned response_size) {
    state_json_formatter_.reset(new (std::nothrow) jsonfmt::SystemStateFormatter());
    configASSERT(state_json_formatter_);

    json_formatter_.reset(new (std::nothrow) fmt::json::DynamicFormatter(response_size));
    configASSERT(json_formatter_);
}

status::StatusCode SystemStateHandler::serve_http(http::IResponseWriter& w,
                                                  http::IRequest&) {
    auto json = fmt::json::CjsonUniqueBuilder::make_object();
    if (!json) {
        return status::StatusCode::NoMem;
    }

    auto code = state_json_formatter_->format(json.get());
    if (code != status::StatusCode::OK) {
        return code;
    }

    code = json_formatter_->format(json.get());
    if (code != status::StatusCode::OK) {
        return code;
    }

    code = algo::ResponseOps::write_json(w, json_formatter_->c_str());
    if (code != status::StatusCode::OK) {
        return code;
    }

    return status::StatusCode::OK;
}

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
