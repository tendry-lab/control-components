/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_http/ihandler.h"
#include "ocs_http/ipattern_iterator.h"
#include "ocs_http/ipattern_matcher.h"
#include "ocs_status/code.h"

namespace ocs {
namespace http {

class IRouter {
public:
    //! Destroy.
    virtual ~IRouter() = default;

    //! Register HTTP handler.
    //!
    //! @remarks
    //!  It's forbidden to register multiple handlers for the same pattern and method.
    virtual void add(IRequest::Method method, const char* pattern, IHandler& handler) = 0;

    //! Receive a registered handler.
    virtual IHandler* match(IRequest::Method method,
                            const char* pattern,
                            size_t match_upto,
                            IPatternMatcher& matcher) = 0;

    //! Iterate over each registered handler.
    virtual void for_each(IRequest::Method method, IPatternIterator& iterator) = 0;
};

} // namespace http
} // namespace ocs
