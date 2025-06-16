/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <ctime>

#include "ocs_core/noncopyable.h"
#include "ocs_http/irouter.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

class TimeHandler : private http::IHandler, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p router to register HTTP endpoint.
    //!  - @p start_point - point in time begin with the system time is valid.
    TimeHandler(http::IRouter& router, time_t start_point);

private:
    status::StatusCode serve_http(http::IResponseWriter& w, http::IRequest& r) override;

    const time_t start_point_ { 0 };
};

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
