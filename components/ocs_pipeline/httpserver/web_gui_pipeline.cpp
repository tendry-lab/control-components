/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstring>
#include <string_view>

#include "ocs_core/file_stream_reader.h"
#include "ocs_http/chunk_stream_writer.h"
#include "ocs_pipeline/httpserver/web_gui_pipeline.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

namespace {

bool check_file_extension(const char* filepath, const char* ext) {
    return std::string_view(filepath).ends_with(ext);
}

const char* parse_content_type(const char* filepath) {
    if (check_file_extension(filepath, ".html")) {
        return "text/html";
    }

    if (check_file_extension(filepath, ".js")) {
        return "application/javascript";
    }

    if (check_file_extension(filepath, ".css")) {
        return "text/css";
    }

    if (check_file_extension(filepath, ".png")) {
        return "image/png";
    }

    if (check_file_extension(filepath, ".ico")) {
        return "image/x-icon";
    }

    if (check_file_extension(filepath, ".svg")) {
        return "text/xml";
    }

    return "text/plain";
}

} // namespace

WebGuiPipeline::WebGuiPipeline(http::IRouter& router,
                               storage::IFsInitializer& fs_initializer,
                               const char* mount_point)
    : mount_point_(mount_point)
    , fs_initializer_(fs_initializer) {
    buffer_.resize(buffer_size_);

    router.add(http::IRouter::Method::Get, "/", *this);
    router.add(http::IRouter::Method::Get, "/dashboard", *this);
    router.add(http::IRouter::Method::Get, "/assets/*", *this);
}

status::StatusCode WebGuiPipeline::serve_http(http::IResponseWriter& w,
                                              http::IRequest& r) {
    if (!strcmp(r.get_uri(), "/")) {
        return handle_root_(w);
    }
    if (!strcmp(r.get_uri(), "/dashboard")) {
        return handle_file_(w, "/index.html");
    }

    return handle_file_(w, r.get_uri());
}

status::StatusCode WebGuiPipeline::handle_root_(http::IResponseWriter& w) {
    auto code = w.write_status(http::StatusCode::MovedPermanently);
    if (code != status::StatusCode::OK) {
        return code;
    }

    code = w.get_header().set("Location", "/dashboard");
    if (code != status::StatusCode::OK) {
        return code;
    }

    // Send headers only.
    return w.write(nullptr, 0);
}

status::StatusCode WebGuiPipeline::handle_file_(http::IResponseWriter& w,
                                                const char* filename) {
    if (!fs_initializer_.valid()) {
        return status::StatusCode::InvalidState;
    }

    std::string file_path = mount_point_;
    file_path += filename;

    auto code = w.get_header().set("Content-Type", parse_content_type(file_path.c_str()));
    if (code != status::StatusCode::OK) {
        return code;
    }

    code = w.get_header().set("Transfer-Encoding", "chunked");
    if (code != status::StatusCode::OK) {
        return code;
    }

    code = w.get_header().set("Content-Encoding", "gzip");
    if (code != status::StatusCode::OK) {
        return code;
    }

    file_path += ".gz";

    core::FileStreamReader reader(file_path.c_str());
    http::ChunkStreamWriter writer(w);
    core::StreamTransceiver transceiver(reader, writer, buffer_);

    return transceiver.transceive();
}

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
