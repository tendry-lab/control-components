/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
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
