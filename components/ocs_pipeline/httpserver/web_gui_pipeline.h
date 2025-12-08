/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <string>
#include <vector>

#include "ocs_core/noncopyable.h"
#include "ocs_core/stream_transceiver.h"
#include "ocs_http/irouter.h"
#include "ocs_storage/ifs_initializer.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

class WebGuiPipeline : private http::IHandler, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p router to register endpoints to serve the Web GUI files.
    //!  - @p fs_initializer to check if FS has been properly initialized and mounted to
    //!    the VFS.
    //!  - @p mount_point - VFS mount point for the Web GUI files.
    WebGuiPipeline(http::IRouter& router,
                   storage::IFsInitializer& fs_initializer,
                   const char* mount_point);

private:
    status::StatusCode serve_http(http::IResponseWriter& w, http::IRequest& r) override;

    status::StatusCode handle_root_(http::IResponseWriter& w);
    status::StatusCode handle_file_(http::IResponseWriter& w, const char* filename);

    static constexpr size_t buffer_size_ = 1024;

    const std::string mount_point_;

    storage::IFsInitializer& fs_initializer_;
    core::StreamTransceiver::Buffer buffer_;
};

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
