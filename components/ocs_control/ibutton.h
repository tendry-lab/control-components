/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_status/code.h"

namespace ocs {
namespace control {

class IButton {
public:
    //! Destroy.
    virtual ~IButton() = default;

    //! Set @p pressed to true if the button is pressed.
    virtual status::StatusCode get_pressed(bool& pressed) = 0;
};

} // namespace control
} // namespace ocs
