/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
