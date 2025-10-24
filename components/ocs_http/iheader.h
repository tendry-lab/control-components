/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_status/code.h"

namespace ocs {
namespace http {

class IHeader {
public:
    //! Destroy.
    virtual ~IHeader() = default;

    //! Set header value.
    virtual status::StatusCode set(const char* key, const char* value) = 0;
};

} // namespace http
} // namespace ocs
