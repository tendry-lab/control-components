/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <algorithm>
#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_http/router.h"

namespace ocs {
namespace http {

void Router::add(IRouter::Method method, const char* pattern, IHandler& handler) {
    auto& endpoints = method_to_endpoints_[method];

    const auto it = std::find_if(endpoints.begin(), endpoints.end(),
                                 [&pattern](const auto& endpoint) {
                                     return endpoint.first == pattern;
                                 });
    configASSERT(it == endpoints.end());

    endpoints.push_back(std::make_pair(pattern, &handler));
}

IHandler* Router::match(IRouter::Method method,
                        const char* pattern_to_match,
                        size_t match_upto,
                        IPatternMatcher& matcher) {
    auto it = method_to_endpoints_.find(method);
    if (it == method_to_endpoints_.end()) {
        return nullptr;
    }

    for (auto& [pattern, handler] : it->second) {
        if (matcher.match_pattern(pattern.c_str(), pattern_to_match, match_upto)) {
            return handler;
        }
    }

    return nullptr;
}

void Router::for_each(Method method, IPatternIterator& iterator) {
    auto it = method_to_endpoints_.find(method);
    if (it != method_to_endpoints_.end()) {
        for (auto& [pattern, handler] : it->second) {
            iterator.iterate_pattern(pattern.c_str(), *handler);
        }
    }
}

} // namespace http
} // namespace ocs
