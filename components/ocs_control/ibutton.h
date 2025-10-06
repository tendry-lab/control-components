/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

namespace ocs {
namespace control {

class IButton {
public:
    //! Destroy.
    virtual ~IButton() = default;

    //! Return true if the button is pressed.
    virtual bool get() = 0;
};

} // namespace control
} // namespace ocs
