/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>

#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/fanout_formatter.h"
#include "ocs_fmt/json/iformatter.h"
#include "ocs_fmt/json/string_formatter.h"
#include "ocs_system/device_info.h"
#include "ocs_system/iarena.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

class RegistrationFormatter : public fmt::json::IFormatter, private core::NonCopyable<> {
public:
    //! Initialize.
    RegistrationFormatter(system::IArena& arena, const system::DeviceInfo& device_info);

    //! Format the underlying data into @p json.
    status::StatusCode format(cJSON* json) override;

    fmt::json::FanoutFormatter& get_fanout_formatter();

private:
    system::UniquePtr<fmt::json::FanoutFormatter> fanout_formatter_;
    system::UniquePtr<fmt::json::IFormatter> toolchain_formatter_;
    system::UniquePtr<fmt::json::StringFormatter> string_formatter_;
};

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
