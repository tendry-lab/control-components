/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_status/code.h"

namespace ocs {
namespace control {

//! LED control interface.
//!
//! @notes
//!  Lock/unlock semantic is an agreement between components, shared the same hardware
//!  resource. When the component is required to access the resource, it should try to
//!  lock the resource with the corresponding priority. The component with a higher
//!  priority should be able to lock the resource, that was previously locked by other
//!  component with lower priority. When the resource is no longer required, it should be
//!  unlocked.
class ILED {
public:
    //! Locking priority.
    enum class Priority : uint8_t {
        //! Initial invalid value.
        None = 0,

        //! General purpose use cases.
        Default,

        //! Unrecoverable error.
        FatalError,

        //! Device locating.
        Locate,

        //! High-priority system events (button pressed, system initialization, FSR).
        System,
    };

    //! Destroy.
    virtual ~ILED() = default;

    //! Mark LED as in use.
    virtual status::StatusCode try_lock(Priority priority) = 0;

    //! Mark LED as free to use.
    virtual status::StatusCode unlock() = 0;

    //! Turn on LED.
    //!
    //! @remarks
    //!  LED should be locked to perform the operation.
    virtual status::StatusCode turn_on() = 0;

    //! Turn off LED.
    //!
    //! @remarks
    //!  LED should be locked to perform the operation.
    virtual status::StatusCode turn_off() = 0;

    //! Flip LED state.
    //!
    //! @remarks
    //!  LED should be locked to perform the operation.
    virtual status::StatusCode flip() = 0;
};

} // namespace control
} // namespace ocs
