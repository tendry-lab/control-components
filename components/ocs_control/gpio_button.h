/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_control/ibutton.h"
#include "ocs_core/noncopyable.h"
#include "ocs_io/gpio/igpio.h"

namespace ocs {
namespace control {

class GpioButton : public IButton, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p gpio to read the GPIO level.
    //!  - @p level - GPIO level when the button is pressed (0 - pull-up, 1 - pull-down).
    GpioButton(io::gpio::IGpio& gpio, int level = 0);

    //! Return true when the button is pressed.
    bool get() override;

private:
    const int level_ { -1 };

    io::gpio::IGpio& gpio_;
};

} // namespace control
} // namespace ocs
