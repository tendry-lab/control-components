/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_status/code.h"

namespace ocs {
namespace http {

class IServer {
public:
    //! Destroy.
    virtual ~IServer() = default;

    //! Start HTTP server.
    virtual status::StatusCode start() = 0;

    //! Stop HTTP server.
    //!
    //! @remarks
    //!  Can be called multiple times.
    virtual status::StatusCode stop() = 0;
};

} // namespace http
} // namespace ocs
