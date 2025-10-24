/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_system/iclock.h"

namespace ocs {
namespace test {

struct TestClock : public system::IClock, private core::NonCopyable<> {
    system::Time now() override;

    system::Time value { 0 };
};

} // namespace test
} // namespace ocs
