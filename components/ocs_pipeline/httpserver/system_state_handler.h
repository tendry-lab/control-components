/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>

#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/dynamic_formatter.h"
#include "ocs_http/ihandler.h"
#include "ocs_http/irouter.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

class SystemStateHandler : private http::IHandler, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p router to register endpoint to receive system statistics.
    //!  - @p response_size - system state response size, in bytes.
    SystemStateHandler(http::IRouter& router, unsigned response_size);

private:
    status::StatusCode serve_http(http::IResponseWriter& w, http::IRequest&) override;

    std::unique_ptr<fmt::json::IFormatter> state_json_formatter_;
    std::unique_ptr<fmt::json::DynamicFormatter> json_formatter_;
};

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
