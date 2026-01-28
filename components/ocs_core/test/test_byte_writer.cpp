/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstring>

#include "unity.h"

#include "ocs_core/byte_writer.h"

namespace ocs {
namespace core {

TEST_CASE("Byte writer: write byte", "[ocs_core], [byte_writer]") {
    uint8_t write_buf[42];
    memset(write_buf, 0, sizeof(write_buf));

    ByteWriter writer(write_buf, sizeof(write_buf));
    TEST_ASSERT_EQUAL(0, writer.get_len());

    for (uint8_t n = 0; n < sizeof(write_buf); ++n) {
        TEST_ASSERT_TRUE(writer.write(n));
    }

    for (uint8_t n = 0; n < sizeof(write_buf); ++n) {
        TEST_ASSERT_EQUAL(n, write_buf[n]);
    }
}

TEST_CASE("Byte writer: write array: whole at once", "[ocs_core], [byte_writer]") {
    uint8_t write_buf[42];
    memset(write_buf, 0, sizeof(write_buf));

    ByteWriter writer(write_buf, sizeof(write_buf));
    TEST_ASSERT_EQUAL(0, writer.get_len());

    uint8_t buf[sizeof(write_buf)];
    memset(buf, 5, sizeof(buf));

    TEST_ASSERT_EQUAL(sizeof(write_buf), writer.write(buf, sizeof(buf)));
    TEST_ASSERT_TRUE(memcmp(write_buf, buf, sizeof(buf)) == 0);
}

TEST_CASE("Byte writer: write array: by parts", "[ocs_core], [byte_writer]") {
    uint8_t write_buf[6];
    memset(write_buf, 0, sizeof(write_buf));

    ByteWriter writer(write_buf, sizeof(write_buf));

    TEST_ASSERT_TRUE(writer.write(static_cast<uint8_t>(5)));
    TEST_ASSERT_EQUAL(1, writer.get_len());

    uint8_t buf[2];
    memset(buf, 3, sizeof(buf));
    TEST_ASSERT_EQUAL(sizeof(buf), writer.write(buf, sizeof(buf)));

    memset(buf, 7, sizeof(buf));
    TEST_ASSERT_EQUAL(sizeof(buf), writer.write(buf, sizeof(buf)));

    // Write the remaining byte
    TEST_ASSERT_TRUE(writer.write(static_cast<uint8_t>(1)));
    TEST_ASSERT_EQUAL(sizeof(write_buf), writer.get_len());

    uint8_t want_buf[] = { 5, 3, 3, 7, 7, 1 };
    TEST_ASSERT_EQUAL(sizeof(want_buf), writer.get_len());
    TEST_ASSERT_TRUE(memcmp(want_buf, writer.get_data(), sizeof(want_buf)) == 0);
}

TEST_CASE("Byte writer: reserve space: in range", "[ocs_core], [byte_writer]") {
    uint8_t write_buf[2];
    memset(write_buf, 7, sizeof(write_buf));

    ByteWriter writer(write_buf, sizeof(write_buf));

    TEST_ASSERT_EQUAL(1, writer.reserve(1));
    TEST_ASSERT_TRUE(writer.write(static_cast<uint8_t>(2)));
    TEST_ASSERT_FALSE(writer.write(static_cast<uint8_t>(2)));
    TEST_ASSERT_EQUAL(7, write_buf[0]);
    TEST_ASSERT_EQUAL(2, write_buf[1]);
}

TEST_CASE("Byte writer: reserve space: no space left", "[ocs_core], [byte_writer]") {
    uint8_t write_buf[2];
    memset(write_buf, 7, sizeof(write_buf));

    ByteWriter writer(write_buf, sizeof(write_buf));

    TEST_ASSERT_EQUAL(writer.get_cap(), writer.reserve(writer.get_cap() + 1));
    TEST_ASSERT_EQUAL(0, writer.reserve(1));
}

} // namespace core
} // namespace ocs
