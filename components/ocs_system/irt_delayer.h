/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_status/code.h"
#include "ocs_system/time.h"

namespace ocs {
namespace system {

//! Provide delay with up to microsecond resolution.
class IRtDelayer {
public:
    //! Destroy.
    virtual ~IRtDelayer() = default;

    //! Pause the execution for @p delay.
    //!
    //! @notes
    //!  The implementation can monopolize CPU time, potentially decreasing the
    //!  performance of other tasks in the system.
    virtual status::StatusCode delay(system::Time delay) = 0;
};

} // namespace system
} // namespace ocs
