/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <charconv>
#include <cstring>
#include <string>

#include "ocs_algo/response_ops.h"
#include "ocs_core/lock_guard.h"
#include "ocs_fmt/json/cjson_builder.h"
#include "ocs_fmt/json/cjson_object_formatter.h"
#include "ocs_fmt/json/dynamic_formatter.h"
#include "ocs_pipeline/httpserver/ap_network_handler.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

ApNetworkHandler::ApNetworkHandler(net::ApNetworkConfig& config,
                                   scheduler::ITask& reboot_task)
    : config_(config)
    , reboot_task_(reboot_task) {
}

status::StatusCode ApNetworkHandler::serve_http(http::IResponseWriter& w,
                                                http::IRequest& r) {
    core::LockGuard lock(mu_);

    const auto values = algo::UriOps::parse_query(r.get_uri());
    if (!values.size()) {
        return handle_get_(w);
    }

    return handle_update_(w, values);
}

status::StatusCode ApNetworkHandler::handle_update_(http::IResponseWriter& w,
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

    code = algo::ResponseOps::write_text(w, "OK");
    if (code != status::StatusCode::OK) {
        return code;
    }

    if (code == status::StatusCode::NotModified) {
        return status::StatusCode::OK;
    }

    return reboot_task_.run();
}

status::StatusCode ApNetworkHandler::handle_get_(http::IResponseWriter& w) {
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
    if (!object_formatter.add_string_ref_cs("password", "")) {
        return status::StatusCode::NoMem;
    }

    fmt::json::DynamicFormatter json_formatter(64);

    auto code = json_formatter.format(json.get());
    if (code != status::StatusCode::OK) {
        return code;
    }

    return algo::ResponseOps::write_json(w, json_formatter.c_str());
}

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
