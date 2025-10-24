/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <vector>

#include "ocs_core/noncopyable.h"
#include "ocs_core/stream_transceiver.h"
#include "ocs_http/irouter.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

class WebGuiPipeline : private http::IHandler, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p router to register endpoints to serve the Web GUI files.
    explicit WebGuiPipeline(http::IRouter& router);

private:
    status::StatusCode serve_http(http::IResponseWriter& w, http::IRequest& r) override;

    void initialize_fs_();
    status::StatusCode handle_root_(http::IResponseWriter& w);
    status::StatusCode handle_file_(http::IResponseWriter& w, const char* filename);

    static constexpr const char* mount_point_ = "/web_gui";
    static constexpr unsigned buffer_size_ = 1024;

    bool valid_ { false };
    core::StreamTransceiver::Buffer buffer_;
};

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
