/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstdint>
#include <sys/types.h>

#include "ocs_status/code.h"

namespace ocs {
namespace http {

class IRequest {
public:
    //! HTTP requests methods.
    enum class Method : uint8_t {
        Unsupported,
        Get,
        Post,
    };

    //! Destroy.
    virtual ~IRequest() = default;

    //! Get the request URI.
    virtual const char* get_uri() const = 0;

    //! Get the request method.
    virtual Method get_method() const = 0;

    //! Return request content length.
    virtual size_t get_content_length() const = 0;

    //! Read @p len bytes of data into @p buf.
    //!
    //! @params
    //!  - @p read_size - number of bytes read.
    //!  - @p buf - buffer to read data.
    //!  - @p buf_size - buffer size, should be at least @p buf_size bytes long.
    virtual status::StatusCode read(size_t& read_size, uint8_t* buf, size_t buf_size) = 0;
};

} // namespace http
} // namespace ocs
