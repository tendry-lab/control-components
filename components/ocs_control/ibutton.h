/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
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
