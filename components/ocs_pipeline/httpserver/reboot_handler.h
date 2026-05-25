/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_http/ihandler.h"
#include "ocs_http/irequest.h"
#include "ocs_http/iresponse_writer.h"
#include "ocs_system/irebooter.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

class RebootHandler : public http::IHandler, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p rebooter to initiate the reboot process.
    explicit RebootHandler(system::IRebooter& rebooter);

    // Reboot system over HTTP.
    status::StatusCode serve_http(http::IResponseWriter& w, http::IRequest& r) override;

private:
    system::IRebooter& rebooter_;
};

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
