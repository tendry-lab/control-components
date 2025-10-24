/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <ctime>

#include "ocs_core/noncopyable.h"
#include "ocs_http/ihandler.h"
#include "ocs_http/irequest.h"
#include "ocs_http/iresponse_writer.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

class TimeHandler : public http::IHandler, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p start_point - point in time begin with the system time is valid.
    explicit TimeHandler(time_t start_point);

    // Set/get system time over HTTP.
    status::StatusCode serve_http(http::IResponseWriter& w, http::IRequest& r) override;

private:
    const time_t start_point_ { 0 };
};

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
