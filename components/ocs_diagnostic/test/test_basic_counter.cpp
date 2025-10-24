/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string>

#include "unity.h"

#include "ocs_diagnostic/basic_counter.h"
#include "ocs_test/test_counter.h"

namespace ocs {
namespace diagnostic {

TEST_CASE("Basic counter: ID length in a range", "[ocs_diagnostic], [basic_counter]") {
    test::TestCounter counter("foo");
    TEST_ASSERT_EQUAL_STRING("foo", counter.id());
}

TEST_CASE("Basic counter: ID length overflow", "[ocs_diagnostic], [basic_counter]") {
    const auto max_len = 15;

    std::string actual_id;
    for (unsigned n = 0; n < max_len * 2; ++n) {
        actual_id += std::to_string(1);
    }

    std::string expected_id;
    for (unsigned n = 0; n < max_len; ++n) {
        expected_id += std::to_string(1);
    }

    TEST_ASSERT_TRUE(actual_id != expected_id);

    test::TestCounter counter(actual_id.c_str());
    TEST_ASSERT_EQUAL_STRING(expected_id.c_str(), counter.id());
}

} // namespace diagnostic
} // namespace ocs
