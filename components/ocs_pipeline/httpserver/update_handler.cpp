/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <charconv>
#include <cstdio>
#include <cstring>
#include <string>

#include "ocs_algo/response_ops.h"
#include "ocs_core/lock_guard.h"
#include "ocs_pipeline/httpserver/update_handler.h"

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

} // namespace

UpdateHandler::UpdateHandler(system::IUpdater& updater, uint32_t max_chunk_size)
    : max_chunk_size_(max_chunk_size)
    , updater_(updater) {
}

status::StatusCode UpdateHandler::serve_http(http::IResponseWriter& w,
                                             http::IRequest& r) {
    core::LockGuard lock(mu_);

    const auto values = algo::UriOps::parse_query(r.get_uri());

    switch (r.get_method()) {
    case http::IRequest::Method::Get:
        return handle_get_(w, r, values);
    case http::IRequest::Method::Post:
        return handle_post_(w, r, values);
    default:
        break;
    }

    return status::StatusCode::InvalidArg;
}

status::StatusCode UpdateHandler::handle_get_(http::IResponseWriter& w,
                                              http::IRequest& r,
                                              const algo::UriOps::Values& values) {
    uint8_t command = 0;
    const auto code = parse_number(command, "command", values);
    if (code != status::StatusCode::OK) {
        return code;
    }
    if (command > 3) {
        return status::StatusCode::InvalidArg;
    }

    switch (static_cast<Command>(command)) {
    case Command::Begin:
        return handle_begin_(w, values);

    case Command::Commit:
        return handle_commit_(w);

    case Command::End:
        return handle_end_(w);

    default:
        break;
    }

    return status::StatusCode::InvalidArg;
}

status::StatusCode UpdateHandler::handle_post_(http::IResponseWriter& w,
                                               http::IRequest& r,
                                               const algo::UriOps::Values& values) {
    if (!buffer_) {
        return status::StatusCode::InvalidState;
    }

    if (r.get_content_length() > buffer_size_) {
        return status::StatusCode::InvalidArg;
    }

    uint32_t seqnum = 0;
    const auto code = parse_number(seqnum, "seqnum", values);
    if (code != status::StatusCode::OK) {
        return code;
    }

    const auto len = r.read(buffer_.get(), buffer_size_);
    if (len <= 0) {
        return status::StatusCode::Error;
    }

    code = updater_.write(buffer_.get(), len);
    if (code != status::StatusCode::OK) {
        return code;
    }

    ocs_logi(log_tag, "chunk successfully written: seqnum=%lu len=%lu", seqnum, len);

    return algo::ResponseOps::write_text(w, std::to_string(seqnum));
}

status::StatusCode UpdateHandler::handle_begin_(http::IResponseWriter& w,
                                                const algo::UriOps::Values& values) {
    if (buffer_) {
        return status::StatusCode::InvalidState;
    }

    uint32_t total_size = 0;
    auto code = parse_number(total_size, "total_size", values);
    if (code != status::StatusCode::OK) {
        return code;
    }

    uint32_t chunk_size = 0;
    code = parse_number(chunk_size, "chunk_size", values);
    if (code != status::StatusCode::OK) {
        return code;
    }
    if (chunk_size > max_chunk_size_) {
        return status::StatusCode::InvalidArg;
    }

    buffer_size_ = chunk_size;

    buffer_.reset(new (std::nothrow) uint8_t[buffer_size_]);
    if (!buffer_) {
        return status::StatusCode::NoMem;
    }

    uint32_t crc32 = 0;
    code = parse_number(crc32, "crc32", values);
    if (code != status::StatusCode::OK) {
        return code;
    }

    code = updater_.begin(total_size, chunk_size, crc32);
    if (code != status::StatusCode::OK) {
        return code;
    }

    return algo::ResponseOps::write_text(w, "OK");
}

status::StatusCode UpdateHandler::handle_commit_(http::IResponseWriter& w) {
    const auto code = updater_.commit();
    if (code != status::StatusCode::OK) {
        return code;
    }

    return algo::ResponseOps::write_text(w, "OK");
}

status::StatusCode UpdateHandler::handle_end_(http::IResponseWriter& w) {
    if (!buffer_) {
        return status::StatusCode::InvalidState;
    }

    auto code = updater_.end();
    if (code != status::StatusCode::OK) {
        return code;
    }

    code = algo::ResponseOps::write_text(w, "OK");
    if (code != status::StatusCode::OK) {
        return code;
    }

    buffer_ = nullptr;

    return status::StatusCode::OK;
}

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
