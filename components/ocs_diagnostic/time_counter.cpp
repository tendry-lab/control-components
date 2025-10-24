/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_diagnostic/time_counter.h"

namespace ocs {
namespace diagnostic {

TimeCounter::TimeCounter(system::IClock& clock, const char* id, system::Time resolution)
    : BasicCounter(id)
    , resolution_(resolution)
    , clock_(clock) {
    configASSERT(resolution_);
}

ICounter::Value TimeCounter::get() const {
    return (clock_.now() - offset_) / resolution_;
}

void TimeCounter::reset() {
    offset_ = clock_.now();
}

} // namespace diagnostic
} // namespace ocs
