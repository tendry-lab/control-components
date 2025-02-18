/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <charconv>
#include <cstring>
#include <string>

#include "ocs_core/log.h"
#include "ocs_fmt/json/cjson_builder.h"
#include "ocs_fmt/json/cjson_object_formatter.h"
#include "ocs_fmt/json/dynamic_formatter.h"
#include "ocs_pipeline/httpserver/ap_network_handler.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

namespace {

const char* log_tag = "ap_network_handler";

} // namespace

ApNetworkHandler::ApNetworkHandler(http::Server& server,
                                   net::ApNetworkConfig& config,
                                   scheduler::ITask& reboot_task)
    : config_(config)
    , reboot_task_(reboot_task) {
    server.add_GET("/api/v1/config/wifi/ap", [this](httpd_req_t* req) {
        const auto values = algo::UriOps::parse_query(req->uri);
        if (!values.size()) {
            return handle_get_(req);
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
        const auto channel = values.find("channel");
        const auto max_conn = values.find("max_conn");

        if (password == values.end() && channel == values.end()
            && max_conn == values.end()) {
            return status::StatusCode::InvalidArg;
        }

        uint8_t parsed_channel = config_.get_channel();

        if (channel != values.end()) {
            const auto result = std::from_chars(
                channel->second.data(), channel->second.data() + channel->second.size(),
                parsed_channel);
            if (result.ec != std::errc()) {
                return status::StatusCode::InvalidArg;
            }
        }

        uint8_t parsed_max_conn = config_.get_max_conn();
        if (max_conn != values.end()) {
            const auto result = std::from_chars(
                max_conn->second.data(),
                max_conn->second.data() + max_conn->second.size(), parsed_max_conn);
            if (result.ec != std::errc()) {
                return status::StatusCode::InvalidArg;
            }
        }

        std::string parsed_password = config_.get_password();
        if (password != values.end()) {
            parsed_password = password->second;
        }

        code =
            config_.configure(parsed_channel, parsed_max_conn, parsed_password.c_str());
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

status::StatusCode ApNetworkHandler::handle_get_(httpd_req_t* req) {
    fmt::json::CjsonUniqueBuilder builder;

    auto json = builder.make_object();
    if (!json) {
        return status::StatusCode::NoMem;
    }

    fmt::json::CjsonObjectFormatter object_formatter(json.get());

    if (!object_formatter.add_number_cs("channel", config_.get_channel())) {
        return status::StatusCode::NoMem;
    }
    if (!object_formatter.add_number_cs("max_conn", config_.get_max_conn())) {
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
        ocs_loge(log_tag, "httpd_resp_send(): %s", esp_err_to_name(err));
        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
