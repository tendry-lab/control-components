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
#include "ocs_status/code.h"

namespace ocs {
namespace http {

class IServer {
public:
    //! Handler to process an HTTP request.
    using HandlerFunc =
        std::function<status::StatusCode(IResponseWriter& w, IRequest& r)>;

    //! Destroy.
    virtual ~IServer() = default;

    //! Start HTTP server.
    virtual status::StatusCode start() = 0;

    //! Stop HTTP server.
    //!
    //! @remarks
    //!  Can be called multiple times.
    virtual status::StatusCode stop() = 0;

    //! Register HTTP handler for a GET request.
    virtual void add_GET(const char* path, HandlerFunc func) = 0;
};

} // namespace http
} // namespace ocs
