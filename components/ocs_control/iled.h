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
class ILED {
public:
    //! Destroy.
    virtual ~ILED() = default;

    //! Mark LED as in use.
    //!
    //! @remarks
    //!  When LED is in use, other components are prohibited to use LED.
    virtual status::StatusCode lock() = 0;

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
