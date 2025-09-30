/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_control/ilocator.h"
#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/iformatter.h"

namespace ocs {
namespace pipeline {
namespace jsonfmt {

class LocateFormatter : public fmt::json::IFormatter, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p locator to get the locating status.
    explicit LocateFormatter(control::ILocator& locator);

    //! Format locating status into @p json.
    status::StatusCode format(cJSON* json) override;

private:
    control::ILocator& locator_;
};

} // namespace jsonfmt
} // namespace pipeline
} // namespace ocs
