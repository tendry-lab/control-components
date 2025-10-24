/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <string>
#include <vector>

#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/cjson_builder.h"
#include "ocs_fmt/json/iformatter.h"

namespace ocs {
namespace fmt {
namespace json {

class FieldFormatter : public IFormatter, private core::NonCopyable<> {
public:
    enum class Type : uint8_t {
        Object,
        Array,
    };

    //! Initialize.
    //!
    //! @params
    //!  - @p key - JSON key.
    //!  - @p type - JSON object type.
    FieldFormatter(const char* key, Type type = Type::Object);

    //! Format data into @p json.
    status::StatusCode format(cJSON* json) override;

    //! Add additional JSON formatter.
    void add(IFormatter& formatter);

private:
    CjsonUniqueBuilder::Ptr make_field_() const;

    const std::string key_;
    const Type type_ { Type::Object };

    std::vector<IFormatter*> formatters_;
};

} // namespace json
} // namespace fmt
} // namespace ocs
