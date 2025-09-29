/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "unity.h"

#include "ocs_net/rssi_to_signal_strength.h"

namespace ocs {
namespace net {

TEST_CASE("Format RSSI string", "[ocs_net], [rssi_to_signal_strength]") {
    TEST_ASSERT_EQUAL_STRING("Excellent", rssi_to_signal_strength(-49));

    TEST_ASSERT_EQUAL_STRING("Good", rssi_to_signal_strength(-50));
    TEST_ASSERT_EQUAL_STRING("Good", rssi_to_signal_strength(-51));
    TEST_ASSERT_EQUAL_STRING("Good", rssi_to_signal_strength(-59));

    TEST_ASSERT_EQUAL_STRING("Fair", rssi_to_signal_strength(-60));
    TEST_ASSERT_EQUAL_STRING("Fair", rssi_to_signal_strength(-62));
    TEST_ASSERT_EQUAL_STRING("Fair", rssi_to_signal_strength(-69));

    TEST_ASSERT_EQUAL_STRING("Poor", rssi_to_signal_strength(-70));
    TEST_ASSERT_EQUAL_STRING("Poor", rssi_to_signal_strength(-73));
    TEST_ASSERT_EQUAL_STRING("Poor", rssi_to_signal_strength(-79));

    TEST_ASSERT_EQUAL_STRING("Unreliable", rssi_to_signal_strength(-80));
    TEST_ASSERT_EQUAL_STRING("Unreliable", rssi_to_signal_strength(-84));
    TEST_ASSERT_EQUAL_STRING("Unreliable", rssi_to_signal_strength(-101));
}

} // namespace net
} // namespace ocs
