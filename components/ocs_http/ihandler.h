/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <functional>

#include "ocs_http/irequest.h"
#include "ocs_http/iresponse_writer.h"

namespace ocs {
namespace http {

//! Handler for an HTTP request.
class IHandler {
public:
    //! Destroy.
    virtual ~IHandler() = default;

    //! Handle HTTP request.
    virtual status::StatusCode serve_http(IResponseWriter& w, IRequest& r) = 0;
};

} // namespace http
} // namespace ocs
