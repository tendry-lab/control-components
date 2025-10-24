/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_system/irt_delayer.h"

namespace ocs {
namespace system {

class RtDelayer : public IRtDelayer, private core::NonCopyable<> {
public:
    //! Highly-accurate delay based on the builtin ets_delay_us().
    status::StatusCode delay(system::Time delay) override;
};

} // namespace system
} // namespace ocs
