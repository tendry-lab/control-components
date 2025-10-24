/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstring>

#include "ocs_core/log.h"
#include "ocs_http/target_esp32/header.h"

namespace ocs {
namespace http {

namespace {

const char* log_tag = "http_header";

} // namespace

Header::Header(httpd_req_t& req)
    : req_(req) {
}

status::StatusCode Header::set(const char* key, const char* value) {
    if (!strcmp(key, "Content-Length")) {
        // ESP-IDF HTTP server implementation implicitly sets the Content-Length header
        // during the httpd_resp_send() call. Just do nothing.
    } else if (!strcmp(key, "Content-Type")) {
        // ESP-IDF HTTP server implementation implicitly sets the Content-Type header
        // during the httpd_resp_send() call.
        const auto err = httpd_resp_set_type(&req_, value);
        if (err != ESP_OK) {
            ocs_loge(log_tag, "httpd_resp_set_hdr(): %s", esp_err_to_name(err));

            return status::StatusCode::Error;
        }
    } else if (!strcmp(key, "Transfer-Encoding") && !strcmp(value, "chunked")) {
        // ESP-IDF HTTP server implementation implicitly sets the Transfer-Encoding header
        // during the httpd_resp_send_chunk() call.
        chunked_ = true;
    } else {
        const auto err = httpd_resp_set_hdr(&req_, key, value);
        if (err != ESP_OK) {
            ocs_loge(log_tag, "httpd_resp_set_hdr(): %s", esp_err_to_name(err));

            return status::StatusCode::Error;
        }
    }

    return status::StatusCode::OK;
}

bool Header::chunked() const {
    return chunked_;
}

} // namespace http
} // namespace ocs
