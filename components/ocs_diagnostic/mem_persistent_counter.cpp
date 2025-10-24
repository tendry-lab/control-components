/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_diagnostic/mem_persistent_counter.h"
#include "ocs_core/log.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace diagnostic {

namespace {

const char* log_tag = "mem_persistent_counter";

} // namespace

MemPersistentCounter::MemPersistentCounter(storage::IStorage& storage, ICounter& counter)
    : BasicPersistentCounter(storage, counter) {
    const auto code = storage.erase(id());
    if (code != status::StatusCode::OK && code != status::StatusCode::NoData) {
        ocs_loge(log_tag, "failed to erase counter value: id=%s code=%s", id(),
                 status::code_to_str(code));
    }
}

ICounter::Value MemPersistentCounter::get() const {
    return value_ + counter_.get();
}

} // namespace diagnostic
} // namespace ocs
