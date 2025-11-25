/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_diagnostic/counter_store.h"

namespace ocs {
namespace diagnostic {

const CounterStore::CounterList& CounterStore::get() const {
    return counters_;
}

void CounterStore::add(ICounter& counter) {
    counters_.emplace_back(&counter);
}

} // namespace diagnostic
} // namespace ocs
