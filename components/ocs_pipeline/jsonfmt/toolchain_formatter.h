/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/iformatter.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

class ToolchainFormatter : public fmt::json::IFormatter, private core::NonCopyable<> {
public:
    // Format toolchain information into @p json.
    status::StatusCode format(cJSON* json) override;
};

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
