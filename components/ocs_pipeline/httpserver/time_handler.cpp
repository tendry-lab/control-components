/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>
#include <string>

#include "ocs_algo/response_ops.h"
#include "ocs_algo/time_ops.h"
#include "ocs_algo/uri_ops.h"
#include "ocs_pipeline/httpserver/time_handler.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

TimeHandler::TimeHandler(time_t start_point)
    : start_point_(start_point) {
}

status::StatusCode TimeHandler::serve_http(http::IResponseWriter& w, http::IRequest& r) {
    const auto values = algo::UriOps::parse_query(r.get_uri());
    const auto it = values.find("timestamp");

    if (it == values.end()) {
        auto timestamp = algo::TimeOps::get_time();
        if (!timestamp) {
            return status::StatusCode::Error;
        }

        if (*timestamp < start_point_) {
            *timestamp = -1;
        }

        return algo::ResponseOps::write_text(w, std::to_string(*timestamp).c_str());
    }

    char buf[it->second.size() + 1];
    memset(buf, 0, sizeof(buf));
    memcpy(buf, it->second.data(), it->second.size());

    const auto code = algo::TimeOps::set_time(buf, start_point_);
    if (code != status::StatusCode::OK) {
        return code;
    }

    return algo::ResponseOps::write_text(w, "OK");
}

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
