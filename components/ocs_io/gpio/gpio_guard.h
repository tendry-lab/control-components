/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_io/gpio/igpio.h"

namespace ocs {
namespace io {
namespace gpio {

class GpioGuard : private core::NonCopyable<> {
public:
    //! Turn on @p gpio on initialization.
    explicit GpioGuard(IGpio& gpio);

    //! Turn off gpio on de-initialization.
    ~GpioGuard();

private:
    IGpio& gpio_;
};

} // namespace gpio
} // namespace io
} // namespace ocs
