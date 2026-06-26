/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_pipeline/jsonfmt/telemetry_formatter.h"
#include "ocs_core/freertos.h"
#include "ocs_pipeline/jsonfmt/system_formatter.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

TelemetryFormatter::TelemetryFormatter(system::IArena& arena) {
    fanout_formatter_ = ocs::system::make_unique_ptr<fmt::json::FanoutFormatter>(arena);
    configASSERT(fanout_formatter_);

    system_formatter_ = ocs::system::make_unique_ptr<SystemFormatter>(arena);
    configASSERT(system_formatter_);

    fanout_formatter_->add(*system_formatter_);
}

status::StatusCode TelemetryFormatter::format(cJSON* json) {
    return fanout_formatter_->format(json);
}

fmt::json::FanoutFormatter& TelemetryFormatter::get_fanout_formatter() {
    return *fanout_formatter_;
}

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
