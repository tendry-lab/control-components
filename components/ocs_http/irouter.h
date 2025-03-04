/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_http/ipath_handler.h"
#include "ocs_http/ipath_iterator.h"
#include "ocs_http/ipath_matcher.h"
#include "ocs_status/code.h"

namespace ocs {
namespace http {

class IRouter {
public:
    //! HTTP endpoint methods.
    enum class Method : uint8_t {
        Get,
    };

    //! Destroy.
    virtual ~IRouter() = default;

    //! Register HTTP handler.
    //!
    //! @remarks
    //!  It's forbidden to register multiple handlers for the same path and method.
    virtual void add(Method method, const char* path, HandlerFunc handler) = 0;

    //! Receive a registered handler.
    virtual HandlerFunc
    match(Method method, const char* path, size_t match_upto, IPathMatcher& matcher) = 0;

    //! Iterate over each registered handler.
    virtual void for_each(Method method, IPathIterator& iterator) = 0;
};

} // namespace http
} // namespace ocs
