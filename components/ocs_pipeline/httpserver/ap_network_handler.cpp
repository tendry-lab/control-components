/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "ocs_pipeline/httpserver/ap_network_handler.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

ApNetworkHandler::ApNetworkHandler(http::Server& server,
                                   net::ApNetworkConfig& config,
                                   scheduler::ITask& reboot_task)
    : config_(config)
    , reboot_task_(reboot_task) {
    server.add_GET("/api/v1/config/wifi/ap", [this](httpd_req_t* req) {
        const auto values = algo::UriOps::parse_query(req->uri);
        if (!values.size()) {
            return status::StatusCode::InvalidArg;
        }

        return handle_update_(req, values);
    });
}

status::StatusCode ApNetworkHandler::handle_update_(httpd_req_t* req,
                                                    const algo::UriOps::Values& values) {
    status::StatusCode code = status::StatusCode::OK;

    const auto reset = values.find("reset");
    if (reset != values.end()) {
        if (reset->second != "1") {
            return status::StatusCode::InvalidArg;
        }

        code = config_.reset();
        if (code != status::StatusCode::OK && code != status::StatusCode::NotModified) {
            return code;
        }
    } else {
        const auto password = values.find("password");
        if (password == values.end()) {
            return status::StatusCode::InvalidArg;
        }

        char password_buf[password->second.size() + 1];
        memset(password_buf, 0, sizeof(password_buf));
        memcpy(password_buf, password->second.data(), password->second.size());

        code = config_.configure(password_buf);
        if (code != status::StatusCode::OK && code != status::StatusCode::NotModified) {
            return code;
        }
    }

    auto err = httpd_resp_set_type(req, HTTPD_TYPE_TEXT);
    if (err != ESP_OK) {
        return status::StatusCode::Error;
    }

    err = httpd_resp_send(req, "OK", HTTPD_RESP_USE_STRLEN);
    if (err != ESP_OK) {
        return status::StatusCode::Error;
    }

    if (code == status::StatusCode::NotModified) {
        return status::StatusCode::OK;
    }

    return reboot_task_.run();
}

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
