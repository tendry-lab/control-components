/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_algo/response_ops.h"
#include "ocs_core/log.h"
#include "ocs_fmt/json/cjson_builder.h"
#include "ocs_pipeline/httpserver/data_handler.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

DataHandler::DataHandler(http::IRouter& router,
                         fmt::json::IFormatter& formatter,
                         const char* path,
                         const char* id,
                         unsigned buffer_size) {
    fanout_formatter_.reset(new (std::nothrow) fmt::json::FanoutFormatter());
    configASSERT(fanout_formatter_);

    fanout_formatter_->add(formatter);

    json_formatter_.reset(new (std::nothrow) fmt::json::DynamicFormatter(buffer_size));
    configASSERT(json_formatter_);

    fanout_formatter_->add(*json_formatter_);

    router.add(http::IRouter::Method::Get, path, *this);
}

status::StatusCode DataHandler::serve_http(http::IResponseWriter& w, http::IRequest&) {
    auto json = fmt::json::CjsonUniqueBuilder::make_object();
    if (!json) {
        return status::StatusCode::NoMem;
    }

    const auto code = fanout_formatter_->format(json.get());
    if (code != status::StatusCode::OK) {
        return code;
    }

    return algo::ResponseOps::write_json(w, json_formatter_->c_str());
}

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
