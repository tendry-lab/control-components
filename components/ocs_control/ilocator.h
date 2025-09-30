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

class ILocator {
public:
    //! Destroy.
    virtual ~ILocator() = default;

    //! Enable device locating.
    virtual status::StatusCode turn_on() = 0;

    //! Disable device locating.
    virtual status::StatusCode turn_off() = 0;

    //! Flip device locating
    virtual status::StatusCode flip() = 0;

    //! Return true if device locating is enabled.
    virtual bool get() const = 0;
};

} // namespace control
} // namespace ocs
