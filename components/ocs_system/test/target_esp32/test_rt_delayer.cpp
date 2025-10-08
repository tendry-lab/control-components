/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
