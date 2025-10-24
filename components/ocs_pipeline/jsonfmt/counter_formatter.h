/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_diagnostic/basic_counter_holder.h"
#include "ocs_fmt/json/iformatter.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

class CounterFormatter : public fmt::json::IFormatter,
                         public diagnostic::BasicCounterHolder,
                         private core::NonCopyable<> {
public:
    //! Format the underlying counters into @p json.
    status::StatusCode format(cJSON* json) override;
};

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
