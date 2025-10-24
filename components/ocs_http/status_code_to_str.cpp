/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_http/status_code_to_str.h"

namespace ocs {
namespace http {

const char* status_code_to_str(StatusCode code) {
    switch (code) {
    case StatusCode::OK:
        return "200 OK";
    case StatusCode::MovedPermanently:
        return "301 Moved Permanently";
    case StatusCode::NotModified:
        return "304 Not Modified";
    case StatusCode::BadRequest:
        return "400 Bad Request";
    case StatusCode::Unauthorized:
        return "401 Unauthorized";
    case StatusCode::Forbidden:
        return "403 Forbidden";
    case StatusCode::NotFound:
        return "404 Not Found";
    case StatusCode::RequestTimeout:
        return "408 Request Timeout";
    case StatusCode::InternalServerError:
        return "500 Internal Server Error";
    case StatusCode::NotImplemented:
        return "501 Not Implemented";
    case StatusCode::ServiceUnavailable:
        return "503 Service Unavailable";
    default:
        return "";
    }

    return "";
}

} // namespace http
} // namespace ocs
