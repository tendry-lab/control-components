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
#include "ocs_scheduler/itask.h"
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
    //!  - @p reboot_task to reboot the system once the firmware update process is
    //!    finished.
    //!  - @p buffer_size - buffer size to read chunks of firmware from HTTP connection.
    UpdateHandler(system::IUpdater& updater,
                  scheduler::ITask& reboot_task,
                  size_t buffer_size);

    // Update device over HTTP.
    status::StatusCode serve_http(http::IResponseWriter& w, http::IRequest& r) override;

private:
    status::StatusCode begin_(http::IRequest& r);
    status::StatusCode write_(http::IRequest& r);

    const size_t buffer_size_ { 0 };

    core::StaticMutex mu_;
    system::IUpdater& updater_;
    scheduler::ITask& reboot_task_;

    std::unique_ptr<uint8_t[]> buffer_;
};

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
