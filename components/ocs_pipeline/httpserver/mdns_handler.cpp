/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "ocs_fmt/json/cjson_builder.h"
#include "ocs_fmt/json/cjson_object_formatter.h"
#include "ocs_fmt/json/dynamic_formatter.h"
#include "ocs_pipeline/httpserver/mdns_handler.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

MdnsHandler::MdnsHandler(http::Server& server,
                         config::MdnsConfig& config,
                         scheduler::ITask& reboot_task)
    : config_(config)
    , reboot_task_(reboot_task) {
    server.add_GET("/api/v1/system/mdns", [this](httpd_req_t* req) {
        const auto values = algo::UriOps::parse_query(req->uri);
        if (!values.size()) {
            return handle_mdns_get_(req);
        }

        return handle_mdns_set_(req, values);
    });
}

status::StatusCode MdnsHandler::handle_mdns_get_(httpd_req_t* req) {
    fmt::json::CjsonUniqueBuilder builder;

    auto json = builder.make_object();
    if (!json) {
        return status::StatusCode::NoMem;
    }

    fmt::json::CjsonObjectFormatter formatter(json.get());
    if (!formatter.add_string_ref_cs("hostname", config_.get_hostname())) {
        return status::StatusCode::NoMem;
    }

    fmt::json::DynamicFormatter json_formatter(64);
    const auto code = json_formatter.format(json.get());
    if (code != status::StatusCode::OK) {
        return code;
    }

    auto err = httpd_resp_set_type(req, HTTPD_TYPE_JSON);
    if (err != ESP_OK) {
        return status::StatusCode::Error;
    }

    err = httpd_resp_send(req, json_formatter.c_str(), HTTPD_RESP_USE_STRLEN);
    if (err != ESP_OK) {
        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

status::StatusCode MdnsHandler::handle_mdns_set_(httpd_req_t* req,
                                                 const algo::UriOps::Values& values) {
    const auto hostname = values.find("hostname");
    if (hostname == values.end()) {
        return status::StatusCode::InvalidArg;
    }

    char hostname_buf[hostname->second.size() + 1];
    memset(hostname_buf, 0, sizeof(hostname_buf));
    memcpy(hostname_buf, hostname->second.data(), hostname->second.size());

    const auto code = config_.configure(hostname_buf);
    if (code != status::StatusCode::OK && code != status::StatusCode::NotModified) {
        return code;
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
