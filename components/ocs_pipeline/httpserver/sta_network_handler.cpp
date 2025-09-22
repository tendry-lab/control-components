/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <charconv>
#include <cstring>
#include <string>

#include "ocs_algo/response_ops.h"
#include "ocs_core/lock_guard.h"
#include "ocs_fmt/json/cjson_builder.h"
#include "ocs_fmt/json/cjson_object_formatter.h"
#include "ocs_fmt/json/dynamic_formatter.h"
#include "ocs_pipeline/httpserver/sta_network_handler.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

StaNetworkHandler::StaNetworkHandler(http::IRouter& router,
                                     net::StaNetworkConfig& config,
                                     scheduler::ITask& reboot_task)
    : config_(config)
    , reboot_task_(reboot_task) {
    router.add(http::IRouter::Method::Get, "/api/v1/config/wifi/sta", *this);
}

status::StatusCode StaNetworkHandler::serve_http(http::IResponseWriter& w,
                                                 http::IRequest& r) {
    core::LockGuard lock(mu_);

    const auto values = algo::UriOps::parse_query(r.get_uri());
    if (!values.size()) {
        return handle_get_(w);
    }

    return handle_update_(w, values);
}

status::StatusCode StaNetworkHandler::handle_update_(http::IResponseWriter& w,
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

    code = algo::ResponseOps::write_text(w, "OK");
    if (code != status::StatusCode::OK) {
        return code;
    }

    if (code == status::StatusCode::NotModified) {
        return status::StatusCode::OK;
    }

    return reboot_task_.run();
}

status::StatusCode StaNetworkHandler::handle_get_(http::IResponseWriter& w) {
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
    if (!object_formatter.add_string_ref_cs("password", "")) {
        return status::StatusCode::NoMem;
    }

    fmt::json::DynamicFormatter json_formatter(72);

    const auto code = json_formatter.format(json.get());
    if (code != status::StatusCode::OK) {
        return code;
    }

    return algo::ResponseOps::write_json(w, json_formatter.c_str());
}

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
