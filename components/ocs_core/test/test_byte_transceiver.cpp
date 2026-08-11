/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstring>

#include "unity.h"

#include "ocs_core/byte_reader.h"
#include "ocs_core/byte_writer.h"

namespace ocs {
namespace core {

TEST_CASE("Test byte read/write", "[ocs_core], [byte_reader], [byte_writer]") {
    const uint32_t w_value = 0x12345678;

    uint8_t w_buf[sizeof(w_value)];
    memset(w_buf, 0, sizeof(w_buf));

    ByteWriter writer(w_buf, sizeof(w_buf));

    TEST_ASSERT_TRUE(writer.write(w_value));

    ByteReader reader(writer.get_data(), writer.get_len());

    uint32_t r_value = 0;
    TEST_ASSERT_TRUE(reader.read(r_value));

    TEST_ASSERT_EQUAL(w_value, r_value);
}

} // namespace core
} // namespace ocs
