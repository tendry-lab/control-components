/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "unity.h"

#include "ocs_system/target_esp32/rt_delayer.h"

namespace ocs {
namespace system {

TEST_CASE("Rt delayer: delay", "[ocs_system], [rt_delayer]") {
    RtDelayer delayer;

    for (unsigned n = 0; n < 10; ++n) {
        delayer.delay(system::Duration::microsecond * 100);
    }
}

} // namespace system
} // namespace ocs
