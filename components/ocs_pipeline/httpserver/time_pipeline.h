/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <ctime>
#include <memory>

#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/fanout_formatter.h"
#include "ocs_pipeline/httpserver/time_handler.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

class TimePipeline : private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p start_point - point in time begin with the system time is valid.
    TimePipeline(fmt::json::FanoutFormatter& telemetry_formatter,
                 fmt::json::FanoutFormatter& registration_formatter,
                 time_t start_point);

    http::IHandler& get_handler();

private:
    std::unique_ptr<fmt::json::IFormatter> formatter_;
    std::unique_ptr<http::IHandler> handler_;
};

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
