/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_test/test_clock.h"

namespace ocs {
namespace test {

system::Time TestClock::now() {
    return value;
}

} // namespace test
} // namespace ocs
