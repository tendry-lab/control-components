/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_core/lock_guard.h"
#include "ocs_core/log.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace core {

namespace {

const char* log_tag = "lock_guard";

} // namespace

LockGuard::LockGuard(ILocker& locker, TickType_t wait)
    : locker_(locker) {
    const auto code = locker_.lock(wait);
    if (unlikely(code != status::StatusCode::OK)) {
        ocs_loge(log_tag, "failed to lock the resource: %s", status::code_to_str(code));
    }
}

LockGuard::~LockGuard() {
    const auto code = locker_.unlock();
    if (unlikely(code != status::StatusCode::OK)) {
        ocs_loge(log_tag, "failed to unlock the resource: %s", status::code_to_str(code));
    }
}

} // namespace core
} // namespace ocs
