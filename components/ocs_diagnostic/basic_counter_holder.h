/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <vector>

#include "ocs_core/noncopyable.h"
#include "ocs_diagnostic/icounter.h"

namespace ocs {
namespace diagnostic {

class BasicCounterHolder : public core::NonCopyable<BasicCounterHolder> {
public:
    using CounterList = std::vector<ICounter*>;

    //! Destroy.
    virtual ~BasicCounterHolder() = default;

    //! Add a new counter.
    void add(ICounter& counter);

protected:
    //! Return the underlying counters.
    const CounterList& get_counters_() const;

private:
    CounterList counters_;
};

} // namespace diagnostic
} // namespace ocs
