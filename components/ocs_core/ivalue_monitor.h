/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_status/code.h"

namespace ocs {
namespace core {

//! Value monitoring.
template <typename T> class IValueMonitor {
public:
    //! Destroy.
    virtual ~IValueMonitor() = default;

    //! Get latest sampled value.
    //!
    //! @remarks
    //!  Can be called from multiple tasks.
    virtual T get() const = 0;

    //! Sample a new value.
    //!
    //! @remarks
    //!  Should be called from one task.
    virtual status::StatusCode update() = 0;
};

} // namespace core
} // namespace ocs
