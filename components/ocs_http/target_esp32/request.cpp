/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_http/target_esp32/request.h"

namespace ocs {
namespace http {

Request::Request(httpd_req_t& req)
    : req_(req) {
}

const char* Request::get_uri() const {
    return req_.uri;
}

} // namespace http
} // namespace ocs
