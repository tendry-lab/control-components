/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstdint>

#include "unity.h"

#include "ocs_core/version_to_str.h"

namespace ocs {
namespace core {

TEST_CASE("Format version: random numbers", "[ocs_core], [version_to_str]") {
    Version version(9, 1, 1);

    version_to_str version_str(version);
    TEST_ASSERT_EQUAL_STRING("9.1.1", version_str.c_str());
}

TEST_CASE("Format version: max numbers", "[ocs_core], [version_to_str]") {
    Version version(UINT16_MAX, UINT16_MAX, UINT16_MAX);

    version_to_str version_str(version);
    TEST_ASSERT_EQUAL_STRING("65535.65535.65535", version_str.c_str());
}

} // namespace core
} // namespace ocs
