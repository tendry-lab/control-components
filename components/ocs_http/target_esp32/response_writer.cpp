/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_http/target_esp32/response_writer.h"
#include "ocs_core/log.h"
#include "ocs_http/status_code_to_str.h"

namespace ocs {
namespace http {

namespace {

const char* log_tag = "response_writer";

} // namespace

ResponseWriter::ResponseWriter(httpd_req_t& req)
    : req_(req)
    , header_(req) {
}

status::StatusCode ResponseWriter::write(const void* data, size_t size) {
    if (header_.chunked()) {
        const auto err =
            httpd_resp_send_chunk(&req_, static_cast<const char*>(data), size);
        if (err != ESP_OK) {
            ocs_loge(log_tag, "httpd_resp_send_chunk(): %s", esp_err_to_name(err));

            return status::StatusCode::Error;
        }
    } else {
        const auto err = httpd_resp_send(&req_, static_cast<const char*>(data), size);
        if (err != ESP_OK) {
            ocs_loge(log_tag, "httpd_resp_send(): %s", esp_err_to_name(err));

            return status::StatusCode::Error;
        }
    }

    return status::StatusCode::OK;
}

status::StatusCode ResponseWriter::write_status(StatusCode code) {
    const auto err = httpd_resp_set_status(&req_, status_code_to_str(code));
    if (err != ESP_OK) {
        ocs_loge(log_tag, "httpd_resp_set_status(): %s", esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

IHeader& ResponseWriter::get_header() {
    return header_;
}

} // namespace http
} // namespace ocs
