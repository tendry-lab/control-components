/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_http/ihandler.h"

namespace ocs {
namespace http {

//! Pattern iterator.
class IPatternIterator {
public:
    //! Destroy.
    virtual ~IPatternIterator() = default;

    //! Handle pattern.
    virtual void iterate_pattern(const char* pattern, IHandler& handler) = 0;
};

} // namespace http
} // namespace ocs
