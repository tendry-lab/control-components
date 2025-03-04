/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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

    static const constexpr char* mount_point_ = "/web_gui";
    static const size_t buffer_size_ = 1024;

    bool valid_ { false };
    core::StreamTransceiver::Buffer buffer_;
};

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
