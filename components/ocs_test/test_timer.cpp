/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_test/test_timer.h"

namespace ocs {
namespace test {

TestTimer::TestTimer(status::StatusCode code)
    : code_(code) {
}

status::StatusCode TestTimer::start() {
    ++start_call_count;

    return code_;
}

status::StatusCode TestTimer::stop() {
    ++stop_call_count;

    return code_;
}

} // namespace test
} // namespace ocs
