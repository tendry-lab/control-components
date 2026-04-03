/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstddef>

namespace ocs {
namespace system {

//! Memory arena.
class IArena {
public:
    //! Destroy.
    virtual ~IArena() = default;

    //! Allocate memory.
    virtual void* allocate(size_t size) = 0;

    //! Deallocate memory.
    virtual void deallocate(void* ptr) = 0;
};

} // namespace system
} // namespace ocs
