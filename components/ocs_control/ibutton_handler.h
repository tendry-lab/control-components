/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/time.h"
#include "ocs_status/code.h"

namespace ocs {
namespace control {

class IButtonHandler {
public:
    //! Destroy.
    virtual ~IButtonHandler() = default;

    //! Handle the button being pressed for @p duration.
    virtual status::StatusCode handle_pressed(core::Time duration) = 0;
};

} // namespace control
} // namespace ocs
