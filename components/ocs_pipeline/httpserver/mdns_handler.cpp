/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "ocs_core/lock_guard.h"
#include "ocs_pipeline/httpserver/mdns_handler.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

MdnsHandler::MdnsHandler(http::Server& server,
                         net::MdnsConfig& config,
                         scheduler::ITask& reboot_task)
    : config_(config)
    , reboot_task_(reboot_task) {
    server.add_GET("/api/v1/config/mdns", [this](httpd_req_t* req) {
        core::LockGuard lock(mu_);

        const auto values = algo::UriOps::parse_query(req->uri);
        if (!values.size()) {
            return status::StatusCode::InvalidArg;
        }

        return handle_update_(req, values);
    });
}

status::StatusCode MdnsHandler::handle_update_(httpd_req_t* req,
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
        const auto hostname = values.find("hostname");
        if (hostname == values.end()) {
            return status::StatusCode::InvalidArg;
        }

        char hostname_buf[hostname->second.size() + 1];
        memset(hostname_buf, 0, sizeof(hostname_buf));
        memcpy(hostname_buf, hostname->second.data(), hostname->second.size());

        code = config_.configure(hostname_buf);
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
