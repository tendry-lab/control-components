/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>

#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/dynamic_formatter.h"
#include "ocs_fmt/json/fanout_formatter.h"
#include "ocs_http/ihandler.h"
#include "ocs_http/irequest.h"
#include "ocs_http/iresponse_writer.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

class DataHandler : public http::IHandler, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p formatter to format the data.
    //!  - @p buffer_size to hold the formatted JSON data, in bytes.
    DataHandler(fmt::json::IFormatter& formatter, unsigned buffer_size);

    // Get data over HTTP.
    status::StatusCode serve_http(http::IResponseWriter& w, http::IRequest&) override;

private:
    std::unique_ptr<fmt::json::FanoutFormatter> fanout_formatter_;
    std::unique_ptr<fmt::json::DynamicFormatter> json_formatter_;
};

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
