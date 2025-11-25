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

class CounterStore : private core::NonCopyable<> {
public:
    using CounterList = std::vector<ICounter*>;

    //! Return the underlying counters.
    const CounterList& get() const;

    //! Add a new counter.
    void add(ICounter& counter);

private:
    CounterList counters_;
};

} // namespace diagnostic
} // namespace ocs
