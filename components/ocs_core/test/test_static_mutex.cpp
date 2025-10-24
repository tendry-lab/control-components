/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "unity.h"

#include "ocs_core/lock_guard.h"
#include "ocs_core/static_mutex.h"

namespace ocs {
namespace core {

TEST_CASE("Static mutex: lock/unlock", "[ocs_core], [static_mutex]") {
    StaticMutex mu;
    LockGuard lock(mu);
}

} // namespace core
} // namespace ocs
