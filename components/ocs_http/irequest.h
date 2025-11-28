/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstdint>
#include <sys/types.h>

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

    //! Return the request content length.
    virtual size_t get_content_length() const = 0;

    //! Read @p len bytes of data into @p buf.
    //!
    //! @return
    //!  Number of bytes read, or -1 if some error occurred.
    virtual ssize_t read(uint8_t* buf, size_t len) = 0;
};

} // namespace http
} // namespace ocs
