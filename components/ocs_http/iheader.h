/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_status/code.h"

namespace ocs {
namespace http {

class IHeader {
public:
    //! Destroy.
    virtual ~IHeader() = default;

    //! Set header value.
    virtual status::StatusCode set(const char* key, const char* value) = 0;
};

} // namespace http
} // namespace ocs
