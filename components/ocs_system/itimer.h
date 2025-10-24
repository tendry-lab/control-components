/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_status/code.h"

namespace ocs {
namespace system {

class ITimer {
public:
    //! Destroy.
    virtual ~ITimer() = default;

    //! Start the timer.
    virtual status::StatusCode start() = 0;

    //! Stop the timer.
    virtual status::StatusCode stop() = 0;
};

} // namespace system
} // namespace ocs
