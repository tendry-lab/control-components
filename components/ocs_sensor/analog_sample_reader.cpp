/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cmath>
#include <limits>

#include "ocs_sensor/analog_sample_reader.h"
#include "ocs_status/macros.h"

namespace ocs {
namespace sensor {

AnalogSampleReader::AnalogSampleReader(system::IRtDelayer& delayer,
                                       io::adc::IReader& reader,
                                       const AnalogConfig& config)
    : config_(config)
    , reader_(reader)
    , delayer_(delayer) {
}

status::StatusCode AnalogSampleReader::read(int& raw) {
    const auto sample_count = config_.get_sample_count();
    if (!sample_count) {
        return status::StatusCode::InvalidState;
    }

    const auto bitwidth = config_.get_bitwidth();
    if (!bitwidth) {
        return status::StatusCode::InvalidState;
    }

    // Check for int overflow.
    const auto max_value = std::pow(2, static_cast<double>(bitwidth));
    const auto max_sample_count = std::numeric_limits<int>::max() / max_value;
    if (max_sample_count < sample_count) {
        return status::StatusCode::InvalidState;
    }

    int total = 0;

    for (size_t n = 0; n < sample_count; ++n) {
        int result = 0;
        OCS_STATUS_RETURN_ON_ERROR(reader_.read(result));

        total += result;

        // Add a short delay to allow the ADC unit to stabilize.
        OCS_STATUS_RETURN_ON_ERROR(delayer_.delay(delay_interval_));
    }

    raw = total / sample_count;

    return status::StatusCode::OK;
}

} // namespace sensor
} // namespace ocs
