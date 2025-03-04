/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_http/ipath_handler.h"

namespace ocs {
namespace http {

//! URI path iterator.
class IPathIterator {
public:
    //! Destroy.
    virtual ~IPathIterator() = default;

    //! Handle URI path.
    virtual void iterate_path(const char* path, HandlerFunc& handler) = 0;
};

} // namespace http
} // namespace ocs
