/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>

#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/fanout_formatter.h"
#include "ocs_fmt/json/iformatter.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

class TelemetryFormatter : public fmt::json::IFormatter, private core::NonCopyable<> {
public:
    //! Initialize.
    TelemetryFormatter();

    //! Format all telemetry data into @p json.
    status::StatusCode format(cJSON* json);

    fmt::json::FanoutFormatter& get_fanout_formatter();

private:
    std::unique_ptr<fmt::json::FanoutFormatter> fanout_formatter_;
    std::unique_ptr<fmt::json::IFormatter> system_formatter_;
};

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
