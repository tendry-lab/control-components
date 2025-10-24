/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "cJSON.h"

#include "ocs_status/code.h"

namespace ocs {
namespace fmt {
namespace json {

class IFormatter {
public:
    //! Destroy.
    virtual ~IFormatter() = default;

    //! Format JSON.
    virtual status::StatusCode format(cJSON* json) = 0;
};

} // namespace json
} // namespace fmt
} // namespace ocs
