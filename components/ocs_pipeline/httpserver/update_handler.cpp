/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <algorithm>
#include <charconv>
#include <cstdio>
#include <cstring>
#include <string>

#include "ocs_algo/response_ops.h"
#include "ocs_core/lock_guard.h"
#include "ocs_core/log.h"
#include "ocs_pipeline/httpserver/update_handler.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

namespace {

template <typename T>
status::StatusCode
parse_number(T& value, const char* key, const algo::UriOps::Values& values) {
    const auto it = values.find(key);
    if (it == values.end()) {
        return status::StatusCode::InvalidArg;
    }

    const auto [_, ec] =
        std::from_chars(it->second.data(), it->second.data() + it->second.size(), value);
    if (ec != std::errc()) {
        return status::StatusCode::InvalidArg;
    }

    if (!value) {
        return status::StatusCode::InvalidArg;
    }

    return status::StatusCode::OK;
}

const char* log_tag = "update_hanlder";

} // namespace

UpdateHandler::UpdateHandler(system::IUpdater& updater,
                             scheduler::ITask& reboot_task,
                             size_t buffer_size)
    : buffer_size_(buffer_size)
    , updater_(updater)
    , reboot_task_(reboot_task) {
}

status::StatusCode UpdateHandler::serve_http(http::IResponseWriter& w,
                                             http::IRequest& r) {
    core::LockGuard lock(mu_);

    auto code = begin_(r);
    if (code != status::StatusCode::OK) {
        return code;
    }

    code = write_(r);
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to write firmware: %s", status::code_to_str(code));

        if (const auto c = updater_.end(); c != status::StatusCode::OK) {
            ocs_loge(log_tag, "failed to end update process on write failure: %s",
                     status::code_to_str(c));
        }

        return code;
    }

    code = updater_.commit();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to commit written firmware: %s",
                 status::code_to_str(code));

        if (const auto c = updater_.end(); c != status::StatusCode::OK) {
            ocs_loge(log_tag, "failed to end update process on commit failure: %s",
                     status::code_to_str(c));
        }

        return code;
    }

    code = updater_.end();
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag, "failed to end update process: %s", status::code_to_str(code));

        return code;
    }

    code = algo::ResponseOps::write_text(w, "OK");
    if (code != status::StatusCode::OK) {
        return code;
    }

    return reboot_task_.run();
}

status::StatusCode UpdateHandler::begin_(http::IRequest& r) {
    const auto values = algo::UriOps::parse_query(r.get_uri());

    size_t total_size = 0;
    auto code = parse_number(total_size, "total_size", values);
    if (code != status::StatusCode::OK) {
        return code;
    }

    uint32_t crc32 = 0;
    code = parse_number(crc32, "crc32", values);
    if (code != status::StatusCode::OK) {
        return code;
    }

    code = updater_.begin(total_size, crc32);
    if (code != status::StatusCode::OK) {
        return code;
    }

    return status::StatusCode::OK;
}

status::StatusCode UpdateHandler::write_(http::IRequest& r) {
    configASSERT(!buffer_);

    buffer_.reset(new (std::nothrow) uint8_t[buffer_size_]);
    if (!buffer_) {
        return status::StatusCode::NoMem;
    }

    status::StatusCode code = status::StatusCode::OK;

    auto remaining = r.get_content_length();

    while (remaining) {
        size_t read_size = 0;

        code = r.read(read_size, buffer_.get(), std::min(remaining, buffer_size_));
        if (code != status::StatusCode::OK) {
            if (code == status::StatusCode::Timeout) {
                continue;
            }

            break;
        }

        code = updater_.write(buffer_.get(), read_size);
        if (code != status::StatusCode::OK) {
            break;
        }

        memset(buffer_.get(), 0, read_size);

        remaining -= read_size;
    }

    buffer_ = nullptr;

    return code;
}

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
