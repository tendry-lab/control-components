/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstring>

#include "unity.h"

#include "ocs_core/byte_reader.h"

namespace ocs {
namespace core {

TEST_CASE("Byte reader: read array: whole at once", "[ocs_core], [byte_reader]") {
    uint8_t write_buf[42];
    memset(write_buf, 5, sizeof(write_buf));

    ByteReader reader(write_buf, sizeof(write_buf));
    TEST_ASSERT_EQUAL(sizeof(write_buf), reader.get_len());

    uint8_t read_buf[sizeof(write_buf)];
    memset(read_buf, 0, sizeof(read_buf));

    TEST_ASSERT_EQUAL(sizeof(write_buf), reader.read(read_buf, sizeof(read_buf)));
    TEST_ASSERT_TRUE(memcmp(write_buf, read_buf, sizeof(write_buf)) == 0);

    TEST_ASSERT_EQUAL(0, reader.get_len());
    TEST_ASSERT_NULL(reader.get_data());
}

TEST_CASE("Byte reader: read byte", "[ocs_core], [byte_reader]") {
    uint8_t write_buf[42];
    memset(write_buf, 5, sizeof(write_buf));

    ByteReader reader(write_buf, sizeof(write_buf));
    TEST_ASSERT_EQUAL(sizeof(write_buf), reader.get_len());

    for (size_t n = 0; n < sizeof(write_buf); ++n) {
        uint8_t byte = 0;
        TEST_ASSERT_TRUE(reader.read(byte));
        TEST_ASSERT_EQUAL(5, byte);
    }

    uint8_t byte = 0;
    TEST_ASSERT_FALSE(reader.read(byte));

    TEST_ASSERT_EQUAL(0, reader.get_len());
    TEST_ASSERT_NULL(reader.get_data());
}

TEST_CASE("Byte reader: read by offset: whole at once", "[ocs_core], [byte_reader]") {
    uint8_t write_buf[42];
    memset(write_buf, 5, sizeof(write_buf));

    ByteReader reader(write_buf, sizeof(write_buf));
    TEST_ASSERT_EQUAL(sizeof(write_buf), reader.get_len());
    TEST_ASSERT_EQUAL_PTR(write_buf, reader.get_data());

    uint8_t byte = 0;
    TEST_ASSERT_TRUE(reader.read(byte));
    TEST_ASSERT_EQUAL(5, byte);
    TEST_ASSERT_EQUAL(sizeof(write_buf) - 1, reader.get_len());

    ByteReader off_reader(reader.get_data(), reader.get_len());
    TEST_ASSERT_EQUAL_PTR(write_buf + 1, off_reader.get_data());
    TEST_ASSERT_EQUAL(sizeof(write_buf) - 1, reader.get_len());
    TEST_ASSERT_TRUE(memcmp(write_buf + 1, off_reader.get_data(), off_reader.get_len())
                     == 0);

    while (off_reader.get_len()) {
        TEST_ASSERT_TRUE(off_reader.read(byte));
        TEST_ASSERT_EQUAL(5, byte);
    }
    TEST_ASSERT_NULL(off_reader.get_data());
}

TEST_CASE("Byte reader: read by offset: read less", "[ocs_core], [byte_reader]") {
    uint8_t write_buf[42];
    memset(write_buf, 5, sizeof(write_buf));

    ByteReader reader(write_buf, sizeof(write_buf));
    TEST_ASSERT_EQUAL(sizeof(write_buf), reader.get_len());

    uint8_t byte = 0;
    TEST_ASSERT_TRUE(reader.read(byte));
    TEST_ASSERT_EQUAL(5, byte);

    TEST_ASSERT_TRUE(reader.read(byte));
    TEST_ASSERT_EQUAL(5, byte);

    TEST_ASSERT_EQUAL(sizeof(write_buf) - 2, reader.get_len());

    uint8_t read_buf[sizeof(write_buf)];
    memset(read_buf, 0, sizeof(read_buf));

    TEST_ASSERT_EQUAL(sizeof(read_buf) - 2, reader.read(read_buf, sizeof(read_buf)));
    TEST_ASSERT_TRUE(memcmp(write_buf + 2, read_buf, sizeof(read_buf) - 2) == 0);

    TEST_ASSERT_FALSE(reader.read(byte));

    ByteReader off_reader(read_buf + sizeof(read_buf) - 2, 2);

    TEST_ASSERT_TRUE(off_reader.read(byte));
    TEST_ASSERT_EQUAL(0, byte);

    TEST_ASSERT_TRUE(off_reader.read(byte));
    TEST_ASSERT_EQUAL(0, byte);

    TEST_ASSERT_FALSE(off_reader.read(byte));
    TEST_ASSERT_EQUAL(0, off_reader.get_len());
    TEST_ASSERT_NULL(off_reader.get_data());
}

} // namespace core
} // namespace ocs
