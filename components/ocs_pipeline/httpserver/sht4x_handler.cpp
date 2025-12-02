/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_pipeline/httpserver/sht4x_handler.h"
#include "ocs_algo/response_ops.h"
#include "ocs_algo/uri_ops.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

SHT4xHandler::SHT4xHandler(scheduler::AsyncFuncScheduler& func_scheduler,
                           sensor::sht4x::Sensor& sensor)
    : func_scheduler_(func_scheduler)
    , sensor_(sensor) {
}

status::StatusCode SHT4xHandler::serve_http(http::IResponseWriter& w, http::IRequest& r) {
    const auto path = algo::UriOps::parse_path(r.get_uri());

    if (path.find("reset") != std::string_view::npos) {
        return handle_operation_(w, [](sensor::sht4x::Sensor& sensor) {
            return sensor.reset();
        });
    }
    if (path.find("heat") != std::string_view::npos) {
        return handle_operation_(w, [](sensor::sht4x::Sensor& sensor) {
            return sensor.heat();
        });
    }

    return status::StatusCode::InvalidArg;
}

status::StatusCode
SHT4xHandler::handle_operation_(http::IResponseWriter& w,
                                SHT4xHandler::HandleOperationFunc func) {
    auto future = func_scheduler_.add([this, func]() {
        return func(sensor_);
    });
    if (!future) {
        return status::StatusCode::InvalidState;
    }
    if (future->wait(wait_op_interval_) != status::StatusCode::OK) {
        return status::StatusCode::Timeout;
    }
    if (future->code() != status::StatusCode::OK) {
        return future->code();
    }

    const auto code = algo::ResponseOps::write_text(w, "OK");
    if (code != status::StatusCode::OK) {
        return code;
    }

    return status::StatusCode::OK;
}

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
