/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>

#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/dynamic_formatter.h"
#include "ocs_http/ihandler.h"
#include "ocs_http/irequest.h"
#include "ocs_http/iresponse_writer.h"
#include "ocs_system/iarena.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

class SystemStateHandler : public http::IHandler, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p arena to perform dynamic allocations.
    //!  - @p response_size - system state response size, in bytes.
    SystemStateHandler(system::IArena& arena, size_t response_size);

    // Get system state over HTTP.
    status::StatusCode serve_http(http::IResponseWriter& w, http::IRequest&) override;

private:
    system::UniquePtr<fmt::json::IFormatter> state_json_formatter_;
    system::UniquePtr<fmt::json::DynamicFormatter> json_formatter_;
};

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
