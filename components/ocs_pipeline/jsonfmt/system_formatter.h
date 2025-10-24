/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/iformatter.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

class SystemFormatter : public fmt::json::IFormatter, private core::NonCopyable<> {
public:
    //! Format system metrics into @p json.
    status::StatusCode format(cJSON* json) override;
};

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
