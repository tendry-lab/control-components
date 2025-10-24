/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_diagnostic/basic_counter.h"

namespace ocs {
namespace test {

struct TestCounter : public diagnostic::BasicCounter, private core::NonCopyable<> {
    explicit TestCounter(const char* id);

    diagnostic::ICounter::Value get() const override;

    diagnostic::ICounter::Value value { 0 };
};

} // namespace test
} // namespace ocs
