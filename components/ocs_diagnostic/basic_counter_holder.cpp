/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_diagnostic/basic_counter_holder.h"

namespace ocs {
namespace diagnostic {

void BasicCounterHolder::add(ICounter& counter) {
    counters_.emplace_back(&counter);
}

const BasicCounterHolder::CounterList& BasicCounterHolder::get_counters_() const {
    return counters_;
}

} // namespace diagnostic
} // namespace ocs
