/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
