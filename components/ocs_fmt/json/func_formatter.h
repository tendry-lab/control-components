/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <functional>

#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/iformatter.h"

namespace ocs {
namespace fmt {
namespace json {

class FuncFormatter : public IFormatter, private core::NonCopyable<> {
public:
    //! Lambda used for JSON formatting.
    using Func = std::function<status::StatusCode(cJSON* json)>;

    //! Initialize
    //!
    //! @params
    //!  @p func to call when format() is called.
    explicit FuncFormatter(Func func);

    //! Call the underlying function to format data into @p json.
    status::StatusCode format(cJSON* json) override;

private:
    Func func_;
};

} // namespace json
} // namespace fmt
} // namespace ocs
