/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstddef>
#include <cstdint>

#include "ocs_status/code.h"

namespace ocs {
namespace security {

class IShaEngine {
public:
    //! Known SHA algorithms.
    enum class Algorithm : uint8_t {
        SHA1,
        SHA256,
        SHA512,
    };

    //! Destroy.
    virtual ~IShaEngine() = default;

    //! Return algorithm used for SHA calculation.
    virtual Algorithm get_algorithm() const = 0;

    //! Generate SHA to @p buf from @p size bytes from @p src.
    //!
    //! @remarks
    //!  - @p src should be at least @p size bytes long.
    //!  - @p buf should be large enough to store the result of SHA calculation.
    virtual status::StatusCode
    generate(uint8_t* buf, const uint8_t* src, size_t size) = 0;
};

} // namespace security
} // namespace ocs
