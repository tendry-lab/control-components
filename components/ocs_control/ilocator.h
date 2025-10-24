/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
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
