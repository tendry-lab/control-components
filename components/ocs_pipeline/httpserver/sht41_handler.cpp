/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_pipeline/httpserver/sht41_handler.h"
#include "ocs_algo/response_ops.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

SHT41Handler::SHT41Handler(scheduler::AsyncFuncScheduler& func_scheduler,
                           http::IRouter& router,
                           sensor::sht41::Sensor& sensor)
    : func_scheduler_(func_scheduler)
    , sensor_(sensor) {
    router.add(http::IRouter::Method::Get, "/api/v1/sensor/sht41/reset",
               [this](http::IResponseWriter& w, http::IRequest& r) {
                   return handle_operation_(w, [](sensor::sht41::Sensor& sensor) {
                       return sensor.reset();
                   });
               });
    router.add(http::IRouter::Method::Get, "/api/v1/sensor/sht41/heat",
               [this](http::IResponseWriter& w, http::IRequest& r) {
                   return handle_operation_(w, [](sensor::sht41::Sensor& sensor) {
                       return sensor.heat();
                   });
               });
}

status::StatusCode
SHT41Handler::handle_operation_(http::IResponseWriter& w,
                                SHT41Handler::HandleOperationFunc func) {
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
