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

#include "ocs_core/lock_guard.h"
#include "ocs_core/log.h"
#include "ocs_fmt/json/cjson_builder.h"
#include "ocs_fmt/json/cjson_object_formatter.h"
#include "ocs_fmt/json/dynamic_formatter.h"
#include "ocs_pipeline/httpserver/sta_network_handler.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

namespace {

const char* log_tag = "sta_network_handler";

} // namespace

StaNetworkHandler::StaNetworkHandler(http::Server& server,
                                     net::StaNetworkConfig& config,
                                     scheduler::ITask& reboot_task)
    : config_(config)
    , reboot_task_(reboot_task) {
    server.add_GET("/api/v1/config/wifi/sta", [this](httpd_req_t* req) {
        core::LockGuard lock(mu_);

        const auto values = algo::UriOps::parse_query(req->uri);
        if (!values.size()) {
            return handle_get_(req);
        }

        return handle_update_(req, values);
    });
}

status::StatusCode StaNetworkHandler::handle_update_(httpd_req_t* req,
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
        const auto max_retry_count = values.find("retry");
        const auto ssid = values.find("ssid");
        const auto password = values.find("password");

        if (max_retry_count == values.end() && ssid == values.end()
            && password == values.end()) {
            return status::StatusCode::InvalidArg;
        }

        uint8_t parsed_max_retry_count = config_.get_max_retry_count();
        if (max_retry_count != values.end()) {
            const auto result = std::from_chars(max_retry_count->second.data(),
                                                max_retry_count->second.data()
                                                    + max_retry_count->second.size(),
                                                parsed_max_retry_count);
            if (result.ec != std::errc()) {
                return status::StatusCode::InvalidArg;
            }
        }

        std::string parsed_ssid = config_.get_ssid();
        if (ssid != values.end()) {
            parsed_ssid = ssid->second;
        }

        std::string parsed_password = config_.get_password();
        if (password != values.end()) {
            parsed_password = password->second;
        }

        code = config_.configure(parsed_max_retry_count, parsed_ssid.c_str(),
                                 parsed_password.c_str());
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

status::StatusCode StaNetworkHandler::handle_get_(httpd_req_t* req) {
    if (!config_.valid()) {
        return status::StatusCode::InvalidState;
    }

    fmt::json::CjsonUniqueBuilder builder;

    auto json = builder.make_object();
    if (!json) {
        return status::StatusCode::NoMem;
    }

    fmt::json::CjsonObjectFormatter object_formatter(json.get());

    if (!object_formatter.add_number_cs("retry", config_.get_max_retry_count())) {
        return status::StatusCode::NoMem;
    }
    if (!object_formatter.add_string_ref_cs("ssid", config_.get_ssid())) {
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
