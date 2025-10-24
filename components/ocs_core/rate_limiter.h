/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_system/iclock.h"
#include "ocs_system/time.h"

namespace ocs {
namespace core {

class RateLimiter : private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p clock to count the time passed since last operation.
    //!  - @p interval - how often the operation may be performed.
    RateLimiter(system::IClock& clock, system::Time interval);

    //! Retur true if an operation can be performed.
    bool allow();

private:
    const system::Time interval_ { 0 };

    system::IClock& clock_;
    system::Time start_ { 0 };
};

} // namespace core
} // namespace ocs
