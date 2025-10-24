/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

namespace ocs {
namespace http {

class IRequest {
public:
    //! Destroy.
    virtual ~IRequest() = default;

    //! Get the request URI.
    virtual const char* get_uri() const = 0;
};

} // namespace http
} // namespace ocs
