/*
 * Copyright (c) 2025, Open Control Systems authors
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

void Router::add(IRouter::Method method, const char* path, HandlerFunc func) {
    auto& endpoints = method_to_endpoints_[method];

    const auto it =
        std::find_if(endpoints.begin(), endpoints.end(), [&path](const auto& endpoint) {
            return endpoint.first == path;
        });
    configASSERT(it == endpoints.end());

    endpoints.push_back(std::make_pair(path, func));
}

HandlerFunc Router::match(IRouter::Method method,
                          const char* path_to_match,
                          size_t match_upto,
                          IPathMatcher& matcher) {
    auto it = method_to_endpoints_.find(method);
    if (it == method_to_endpoints_.end()) {
        return nullptr;
    }

    for (auto& [ref_path, handler] : it->second) {
        if (matcher.match_path(ref_path.c_str(), path_to_match, match_upto)) {
            return handler;
        }
    }

    return nullptr;
}

void Router::for_each(Method method, IPathIterator& iterator) {
    auto it = method_to_endpoints_.find(method);
    if (it != method_to_endpoints_.end()) {
        for (auto& [path, handler] : it->second) {
            iterator.iterate_path(path.c_str(), handler);
        }
    }
}

} // namespace http
} // namespace ocs
