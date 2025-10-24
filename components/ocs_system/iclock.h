/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_system/time.h"

namespace ocs {
namespace system {

class IClock {
public:
    //! Destroy.
    virtual ~IClock() = default;

    //! Get time in microseconds since boot.
    virtual system::Time now() = 0;
};

} // namespace system
} // namespace ocs
