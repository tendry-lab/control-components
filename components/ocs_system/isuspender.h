/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_status/code.h"

namespace ocs {
namespace system {

class ISuspender {
public:
    //! Destroy.
    virtual ~ISuspender() = default;

    //! Suspend the system.
    virtual status::StatusCode suspend() = 0;

    //! Resume the system.
    virtual status::StatusCode resume() = 0;
};

} // namespace system
} // namespace ocs
