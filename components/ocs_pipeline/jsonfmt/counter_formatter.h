/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_diagnostic/counter_store.h"
#include "ocs_fmt/json/iformatter.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

class CounterFormatter : public fmt::json::IFormatter, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p store with all counters.
    explicit CounterFormatter(diagnostic::CounterStore& store);

    //! Format the underlying counters into @p json.
    status::StatusCode format(cJSON* json) override;

private:
    diagnostic::CounterStore& store_;
};

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
