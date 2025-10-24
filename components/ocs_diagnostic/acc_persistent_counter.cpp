/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_diagnostic/acc_persistent_counter.h"

namespace ocs {
namespace diagnostic {

AccPersistentCounter::AccPersistentCounter(storage::IStorage& storage, ICounter& counter)
    : BasicPersistentCounter(storage, counter) {
}

ICounter::Value AccPersistentCounter::get() const {
    return value_ + counter_.get();
}

} // namespace diagnostic
} // namespace ocs
