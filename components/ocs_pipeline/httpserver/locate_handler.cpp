/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <charconv>
#include <cstring>
#include <string>

#include "ocs_algo/response_ops.h"
#include "ocs_algo/uri_ops.h"
#include "ocs_pipeline/httpserver/locate_handler.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

LocateHandler::LocateHandler(control::ILocator& locator)
    : locator_(locator) {
}

status::StatusCode LocateHandler::serve_http(http::IResponseWriter& w,
                                             http::IRequest& r) {
    const auto values = algo::UriOps::parse_query(r.get_uri());
    const auto it = values.find("value");

    if (it == values.end()) {
        return algo::ResponseOps::write_text(w, std::to_string(locator_.get()).c_str());
    }

    int value = -1;

    const auto [_, ec] =
        std::from_chars(it->second.data(), it->second.data() + it->second.size(), value);
    if (ec != std::errc()) {
        return status::StatusCode::InvalidArg;
    }

    if (value < 0 || value > 2) {
        return status::StatusCode::InvalidArg;
    }

    status::StatusCode code = status::StatusCode::Error;

    if (value == 0) {
        code = locator_.turn_off();
    }
    if (value == 1) {
        code = locator_.turn_on();
    }
    if (value == 2) {
        code = locator_.flip();
    }

    if (code != status::StatusCode::OK) {
        return code;
    }

    return algo::ResponseOps::write_text(w, "OK");
}

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
