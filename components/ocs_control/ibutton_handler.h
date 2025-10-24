/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_status/code.h"
#include "ocs_system/time.h"

namespace ocs {
namespace control {

class IButtonHandler {
public:
    //! Destroy.
    virtual ~IButtonHandler() = default;

    //! Handle the button being pressed for @p duration.
    virtual status::StatusCode handle_pressed(system::Time duration) = 0;
};

} // namespace control
} // namespace ocs
