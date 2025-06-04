/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "ocs_core/noncopyable.h"
#include "ocs_http/irouter.h"

namespace ocs {
namespace http {

class Router : public IRouter, private core::NonCopyable<> {
public:
    //! Register HTTP endpoint.
    void add(Method method, const char* pattern, IHandler& handler) override;

    //! Receive a registered handler.
    IHandler* match(Method method,
                    const char* pattern,
                    size_t match_upto,
                    IPatternMatcher& matcher) override;

    //! Iterate over each registered handler.
    void for_each(Method method, IPatternIterator& iterator) override;

private:
    using Endpoint = std::pair<std::string, IHandler*>;
    using EndpointList = std::vector<Endpoint>;

    std::unordered_map<Method, EndpointList> method_to_endpoints_;
};

} // namespace http
} // namespace ocs
