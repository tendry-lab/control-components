/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

namespace ocs {
namespace http {

//! HTTP status codes.
enum class StatusCode {
    // RFC 9110, 15.3.1
    OK = 200,
    // RFC 9110, 15.4.2
    MovedPermanently = 301,
    // RFC 9110, 15.4.5
    NotModified = 304,

    // RFC 9110, 15.5.1
    BadRequest = 400,
    // RFC 9110, 15.5.2
    Unauthorized = 401,
    // RFC 9110, 15.5.4
    Forbidden = 403,
    // RFC 9110, 15.5.5
    NotFound = 404,
    // RFC 9110, 15.5.9
    RequestTimeout = 408,

    // RFC 9110, 15.6.1
    InternalServerError = 500,
    // RFC 9110, 15.6.2
    NotImplemented = 501,
    // RFC 9110, 15.6.4
    ServiceUnavailable = 503,
};

} // namespace http
} // namespace ocs
