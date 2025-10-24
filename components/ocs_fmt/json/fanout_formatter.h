/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <vector>

#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/iformatter.h"

namespace ocs {
namespace fmt {
namespace json {

class FanoutFormatter : public IFormatter, private core::NonCopyable<> {
public:
    //! Propogate the call to the underlying formatters.
    status::StatusCode format(cJSON* json) override;

    //! Add @p formatter to be notified when format is called.
    void add(IFormatter& formatter);

private:
    std::vector<IFormatter*> formatters_;
};

} // namespace json
} // namespace fmt
} // namespace ocs
