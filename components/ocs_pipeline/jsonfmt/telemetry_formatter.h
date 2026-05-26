/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>

#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/fanout_formatter.h"
#include "ocs_fmt/json/iformatter.h"
#include "ocs_system/iarena.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

class TelemetryFormatter : public fmt::json::IFormatter, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p arena to perform dynamic allocations.
    explicit TelemetryFormatter(system::IArena& arena);

    //! Format all telemetry data into @p json.
    status::StatusCode format(cJSON* json);

    fmt::json::FanoutFormatter& get_fanout_formatter();

private:
    system::UniquePtr<fmt::json::FanoutFormatter> fanout_formatter_;
    system::UniquePtr<fmt::json::IFormatter> system_formatter_;
};

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
