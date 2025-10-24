/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstdint>

namespace ocs {
namespace diagnostic {

class ICounter {
public:
    using Value = uint64_t;

    //! Destroy.
    virtual ~ICounter() = default;

    //! Return a unique identifier to distinguish one counter from another.
    virtual const char* id() const = 0;

    //! Return the current counter value.
    virtual Value get() const = 0;
};

} // namespace diagnostic
} // namespace ocs
