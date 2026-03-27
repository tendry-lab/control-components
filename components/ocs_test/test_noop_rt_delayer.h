/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_system/irt_delayer.h"

namespace ocs {
namespace test {

class TestNoopRtDelayer : public system::IRtDelayer, private core::NonCopyable<> {
public:
    status::StatusCode delay(system::Time delay) {
        return status::StatusCode::OK;
    }
};

} // namespace test
} // namespace ocs
