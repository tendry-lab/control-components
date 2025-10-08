/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_system/iclock.h"

namespace ocs {
namespace system {

class DefaultClock : public IClock, private core::NonCopyable<> {
public:
    //! Return time in microseconds since boot.
    system::Time now() override;
};

} // namespace system
} // namespace ocs
