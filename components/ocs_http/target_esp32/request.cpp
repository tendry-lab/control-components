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

IRequest::Method Request::get_method() const {
    switch (req_.method) {
    case HTTP_GET:
        return IRequest::Method::Get;
    case HTTP_POST:
        return IRequest::Method::Post;

    default:
        break;
    }

    return IRequest::Method::Unsupported;
}

size_t Request::get_content_length() const {
    return req_.content_len;
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
