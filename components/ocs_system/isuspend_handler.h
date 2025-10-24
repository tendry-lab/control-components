/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_status/code.h"

namespace ocs {
namespace system {

//! At some point of time the system may need more resources, than it currently has.
//! In this case it can ask the implementation of this interface to release some memory
//! or CPU usage, thus allowing the system to perform some heavy operations.
class ISuspendHandler {
public:
    //! Destroy.
    virtual ~ISuspendHandler() = default;

    //! Suspend the component.
    //!
    //! @notes
    //!  It is a good place to release memory and CPU usage.
    virtual status::StatusCode handle_suspend() = 0;

    //! Resume the component.
    virtual status::StatusCode handle_resume() = 0;
};

} // namespace system
} // namespace ocs
