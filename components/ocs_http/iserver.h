/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
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
