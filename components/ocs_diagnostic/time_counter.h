/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_diagnostic/basic_counter.h"
#include "ocs_system/iclock.h"
#include "ocs_system/time.h"

namespace ocs {
namespace diagnostic {

class TimeCounter : public BasicCounter, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p clock to read a time since boot.
    //!  - @p id - counter identifier.
    //!  - @p resolution - time resolution used to count the value.
    TimeCounter(system::IClock& clock, const char* id, system::Time resolution);

    //! Return time since boot with the configured resolution.
    ICounter::Value get() const override;

    //! Restart time counting from now.
    void reset();

private:
    const system::Time resolution_ = system::Duration::microsecond;

    system::IClock& clock_;

    system::Time offset_ { 0 };
};

} // namespace diagnostic
} // namespace ocs
