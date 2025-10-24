/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_test/test_counter.h"

namespace ocs {
namespace test {

TestCounter::TestCounter(const char* id)
    : diagnostic::BasicCounter(id) {
}

diagnostic::ICounter::Value TestCounter::get() const {
    return value;
}

} // namespace test
} // namespace ocs
