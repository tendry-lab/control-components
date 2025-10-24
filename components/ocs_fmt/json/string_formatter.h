/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <string>
#include <unordered_map>

#include "ocs_core/noncopyable.h"
#include "ocs_core/version.h"
#include "ocs_fmt/json/iformatter.h"

namespace ocs {
namespace fmt {
namespace json {

class StringFormatter : public IFormatter, public core::NonCopyable<StringFormatter> {
public:
    //! Destroy.
    virtual ~StringFormatter() = default;

    //! Format key-value pairs into @p json.
    status::StatusCode format(cJSON* json) override;

    //! Add key-value pair to be added to the result json when format() is called.
    void add(const char* key, const char* value);

private:
    std::unordered_map<std::string, std::string> values_;
};

} // namespace json
} // namespace fmt
} // namespace ocs
