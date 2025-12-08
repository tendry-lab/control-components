/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstring>

#include "ocs_algo/uri_ops.h"
#include "ocs_core/log.h"
#include "ocs_http/target_esp32/request.h"
#include "ocs_http/target_esp32/response_writer.h"
#include "ocs_http/target_esp32/server.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace http {

namespace {

const char* log_tag = "http_server";

httpd_err_code_t status_code_to_http_code(status::StatusCode code) {
    switch (code) {
    case status::StatusCode::InvalidArg:
        return HTTPD_400_BAD_REQUEST;

    default:
        break;
    }

    return HTTPD_500_INTERNAL_SERVER_ERROR;
}

} // namespace

Server::Server(IRouter& router, Params params)
    : router_(router) {
    config_ = HTTPD_DEFAULT_CONFIG();
    config_.server_port = params.server_port;
    config_.max_uri_handlers = params.max_uri_handlers;
    config_.uri_match_fn = httpd_uri_match_wildcard;
}

Server::~Server() {
    if (handle_) {
        stop();
    }
}

status::StatusCode Server::start() {
    const auto err = httpd_start(&handle_, &config_);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "httpd_start(): %s", esp_err_to_name(err));
        return status::StatusCode::Error;
    }

    return register_uris_();
}

status::StatusCode Server::register_uris_() {
    router_.for_each(IRequest::Method::Get, *this);
    router_.for_each(IRequest::Method::Post, *this);

    return status::StatusCode::OK;
}

status::StatusCode Server::stop() {
    if (handle_) {
        ESP_ERROR_CHECK(httpd_stop(handle_));
        handle_ = nullptr;
    }

    return status::StatusCode::OK;
}

esp_err_t Server::handle_request_cb_(httpd_req_t* req) {
    Server& self = *static_cast<Server*>(req->user_ctx);
    self.handle_request_(req);

    return ESP_OK;
}

bool Server::match_pattern(const char* reference_pattern,
                           const char* pattern_to_match,
                           size_t match_upto) {
    if (config_.uri_match_fn) {
        return config_.uri_match_fn(reference_pattern, pattern_to_match, match_upto);
    }

    return strcmp(reference_pattern, pattern_to_match) == 0;
}

void Server::iterate_pattern(IRequest::Method method, const char* path, IHandler&) {
    httpd_uri_t uri;
    memset(&uri, 0, sizeof(uri));

    switch (method) {
    case IRequest::Method::Get:
        uri.method = HTTP_GET;
        break;

    case IRequest::Method::Post:
        uri.method = HTTP_POST;
        break;

    default:
        uri.method = static_cast<httpd_method_t>(-1);
        break;
    }

    uri.handler = handle_request_cb_;
    uri.user_ctx = this;
    uri.uri = path;

    ESP_ERROR_CHECK(httpd_register_uri_handler(handle_, &uri));
}

void Server::handle_request_(httpd_req_t* req) {
    const auto path = algo::UriOps::parse_path(req->uri);

    Request r(*req);

    auto handler = router_.match(r.get_method(), path.data(), path.size(), *this);
    if (!handler) {
        ocs_loge(log_tag, "unknown URI: %s", req->uri);

        const auto ret = httpd_resp_send_err(
            req, status_code_to_http_code(status::StatusCode::InvalidArg),
            status::code_to_str(status::StatusCode::InvalidArg));
        if (ret != ESP_OK) {
            ocs_loge(log_tag, "httpd_resp_send_err(): URI=%s err=%s", req->uri,
                     esp_err_to_name(ret));
        }

        return;
    }

    ResponseWriter w(*req);

    const auto code = handler->serve_http(w, r);
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to handle request: URI=%s code=%s", req->uri,
                 status::code_to_str(code));

        const auto http_code = status_code_to_http_code(code);
        const auto ret = httpd_resp_send_err(req, http_code, status::code_to_str(code));
        if (ret != ESP_OK) {
            ocs_loge(log_tag, "httpd_resp_send_err(): URI=%s err=%s", req->uri,
                     esp_err_to_name(ret));
        }
    }
}

} // namespace http
} // namespace ocs
