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

status::StatusCode Request::read(size_t& read_size, uint8_t* buf, size_t buf_size) {
    const auto ret = httpd_req_recv(&req_, reinterpret_cast<char*>(buf), buf_size);
    if (ret <= 0) {
        if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
            return status::StatusCode::Timeout;
        }

        return status::StatusCode::Error;
    }

    read_size = ret;

    return status::StatusCode::OK;
}

} // namespace http
} // namespace ocs
