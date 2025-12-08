/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstdint>
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
};

} // namespace http
} // namespace ocs
