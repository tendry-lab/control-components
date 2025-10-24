/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
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
