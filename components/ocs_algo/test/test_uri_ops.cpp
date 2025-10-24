/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "unity.h"

#include "ocs_algo/uri_ops.h"

namespace ocs {
namespace algo {

TEST_CASE("URI ops: parse path: null input", "[ocs_algo], [uri_ops]") {
    TEST_ASSERT_TRUE(UriOps::parse_path(nullptr) == std::string_view());
}

TEST_CASE("URI ops: parse path: empty", "[ocs_algo], [uri_ops]") {
    TEST_ASSERT_TRUE(UriOps::parse_path("") == std::string_view());
}

TEST_CASE("URI ops: parse path: without query", "[ocs_algo], [uri_ops]") {
    TEST_ASSERT_TRUE(UriOps::parse_path("/foo/bar/baz") == "/foo/bar/baz");
}

TEST_CASE("URI ops: parse path: with query", "[ocs_algo], [uri_ops]") {
    TEST_ASSERT_TRUE(UriOps::parse_path("/foo/bar/baz?key=val") == "/foo/bar/baz");
}

TEST_CASE("URI ops: parse query: null input", "[ocs_algo], [uri_ops]") {
    TEST_ASSERT_EQUAL(0, UriOps::parse_query(nullptr).size());
}

TEST_CASE("URI ops: parse query: empty inut", "[ocs_algo], [uri_ops]") {
    TEST_ASSERT_EQUAL(0, UriOps::parse_query("").size());
}

TEST_CASE("URI ops: parse query: no query", "[ocs_algo], [uri_ops]") {
    TEST_ASSERT_EQUAL(0, UriOps::parse_query("/foo/bar").size());
}

TEST_CASE("URI ops: parse query: key missed", "[ocs_algo], [uri_ops]") {
    TEST_ASSERT_EQUAL(0, UriOps::parse_query("/foo/bar?").size());
}

TEST_CASE("URI ops: parse query: wrong delimiter", "[ocs_algo], [uri_ops]") {
    TEST_ASSERT_EQUAL(0, UriOps::parse_query("/foo/bar?key:").size());
}

TEST_CASE("URI ops: parse query: key-value missed", "[ocs_algo], [uri_ops]") {
    TEST_ASSERT_EQUAL(0, UriOps::parse_query("/foo/bar?=").size());
}

TEST_CASE("URI ops: parse query: value missed", "[ocs_algo], [uri_ops]") {
    TEST_ASSERT_EQUAL(0, UriOps::parse_query("/foo/bar?key=").size());
}

TEST_CASE("URI ops: parse query: single pair", "[ocs_algo], [uri_ops]") {
    const auto values = UriOps::parse_query("/foo/bar?key=value");
    TEST_ASSERT_EQUAL(1, values.size());

    const auto it = values.find("key");
    TEST_ASSERT_TRUE(it != values.end());
    TEST_ASSERT_TRUE(it->first == "key");
    TEST_ASSERT_TRUE(it->second == "value");
}

TEST_CASE("URI ops: parse query: numeric argument", "[ocs_algo], [uri_ops]") {
    const auto values = UriOps::parse_query("/foo/bar?key=2");
    TEST_ASSERT_EQUAL(1, values.size());

    const auto it = values.find("key");
    TEST_ASSERT_TRUE(it != values.end());
    TEST_ASSERT_TRUE(it->first == "key");
    TEST_ASSERT_TRUE(it->second == "2");
}

TEST_CASE("URI ops: parse query: multile pairs", "[ocs_algo], [uri_ops]") {
    const auto values = UriOps::parse_query("/foo/bar?key1=value1&key2=value2");
    TEST_ASSERT_EQUAL(2, values.size());

    auto it = values.find("key1");
    TEST_ASSERT_TRUE(it != values.end());
    TEST_ASSERT_TRUE(it->first == "key1");
    TEST_ASSERT_TRUE(it->second == "value1");

    it = values.find("key2");
    TEST_ASSERT_TRUE(it != values.end());
    TEST_ASSERT_TRUE(it->first == "key2");
    TEST_ASSERT_TRUE(it->second == "value2");
}

} // namespace algo
} // namespace ocs
