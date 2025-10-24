/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_pipeline/jsonfmt/system_formatter.h"
#include "ocs_pipeline/jsonfmt/telemetry_formatter.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

TelemetryFormatter::TelemetryFormatter() {
    fanout_formatter_.reset(new (std::nothrow) fmt::json::FanoutFormatter());
    configASSERT(fanout_formatter_);

    system_formatter_.reset(new (std::nothrow) SystemFormatter());
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
