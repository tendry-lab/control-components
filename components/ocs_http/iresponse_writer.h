/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_http/iheader.h"
#include "ocs_http/status_code.h"
#include "ocs_status/code.h"

namespace ocs {
namespace http {

class IResponseWriter {
public:
    //! Destroy.
    virtual ~IResponseWriter() = default;

    //! Write the data to the connection as part of an HTTP reply.
    //!
    //! @remarks
    //!  - Respond with empty data to signal HTTP response completion: write(nullptr, 0).
    virtual status::StatusCode write(const void* data, size_t size) = 0;

    //! Write HTTP status code.
    virtual status::StatusCode write_status(StatusCode) = 0;

    //! Return the associated HTTP header.
    virtual IHeader& get_header() = 0;
};

} // namespace http
} // namespace ocs
