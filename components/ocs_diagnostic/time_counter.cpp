/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
