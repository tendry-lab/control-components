/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
