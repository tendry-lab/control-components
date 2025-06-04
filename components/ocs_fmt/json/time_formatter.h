/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <ctime>

#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/iformatter.h"

namespace ocs {
namespace fmt {
namespace json {

class TimeFormatter : public IFormatter, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p start_point - point in time begin with the system time is valid.
    explicit TimeFormatter(time_t start_point);

    //! Format system time to @p json.
    status::StatusCode format(cJSON* json) override;

private:
    const time_t start_point_ { -1 };
};

} // namespace json
} // namespace fmt
} // namespace ocs
