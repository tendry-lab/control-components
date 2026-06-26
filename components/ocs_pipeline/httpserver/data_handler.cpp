/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_pipeline/httpserver/data_handler.h"
#include "ocs_algo/response_ops.h"
#include "ocs_core/freertos.h"
#include "ocs_core/log.h"
#include "ocs_fmt/json/cjson_builder.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

DataHandler::DataHandler(system::IArena& arena,
                         fmt::json::IFormatter& formatter,
                         size_t buffer_size) {
    fanout_formatter_ = ocs::system::make_unique_ptr<fmt::json::FanoutFormatter>(arena);
    configASSERT(fanout_formatter_);

    fanout_formatter_->add(formatter);

    json_formatter_ = ocs::system::make_unique_ptr<fmt::json::DynamicFormatter>(
        arena, arena, buffer_size);
    configASSERT(json_formatter_);

    fanout_formatter_->add(*json_formatter_);
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
