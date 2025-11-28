/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>

#include "ocs_algo/uri_ops.h"
#include "ocs_core/noncopyable.h"
#include "ocs_core/static_mutex.h"
#include "ocs_http/ihandler.h"
#include "ocs_http/irequest.h"
#include "ocs_http/iresponse_writer.h"
#include "ocs_system/iupdater.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

class UpdateHandler : public http::IHandler, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p updater to perform the firmware update process.
    //!  - @p max_chunk_size - maximum firmware chunk size, in bytes.
    UpdateHandler(system::IUpdater& updater, uint32_t max_chunk_size);

    // Update device over HTTP.
    status::StatusCode serve_http(http::IResponseWriter& w, http::IRequest& r) override;

private:
    enum class Command {
        Begin = 1,
        Commit = 2,
        End = 3,
    };

    status::StatusCode handle_get_(http::IResponseWriter& w,
                                   http::IRequest& r,
                                   const algo::UriOps::Values& values);

    status::StatusCode handle_post_(http::IResponseWriter& w,
                                    http::IRequest& r,
                                    const algo::UriOps::Values& values);

    status::StatusCode handle_begin_(http::IResponseWriter& w,
                                     const algo::UriOps::Values& values);

    status::StatusCode handle_end_(http::IResponseWriter& w);
    status::StatusCode handle_commit_(http::IResponseWriter& w);

    const uint32_t max_chunk_size_ { 0 };

    core::StaticMutex mu_;
    system::IUpdater& updater_;

    uint32_t buffer_size_ { 0 };
    std::unique_ptr<uint8_t[]> buffer_;
};

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
