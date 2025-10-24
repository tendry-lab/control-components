/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_system/itimer.h"

namespace ocs {
namespace test {

class TestTimer : public system::ITimer, private core::NonCopyable<> {
public:
    explicit TestTimer(status::StatusCode code);

    status::StatusCode start() override;
    status::StatusCode stop() override;

    unsigned start_call_count { 0 };
    unsigned stop_call_count { 0 };

private:
    status::StatusCode code_ { status::StatusCode::OK };
};

} // namespace test
} // namespace ocs
