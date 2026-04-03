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

    //! Deallocate previously allocated memory.
    virtual void deallocate(void* ptr) = 0;
};

} // namespace system
} // namespace ocs

//! Placement new for system::IArena.
//!
//! @note
//!  nothrow forces compiler to check for NULL return value before calling ctor.
inline void* operator new(size_t size, ocs::system::IArena& arena) throw() {
    return arena.allocate(size);
}

//! Placement new[] for system::IArena.
//!
//! @note
//!  nothrow forces compiler to check for NULL return value before calling ctor.
inline void* operator new[](size_t size, ocs::system::IArena& arena) throw() {
    return arena.allocate(size);
}

//! Placement delete for system::IArena.
//!
//! @note
//!  Compiler calls this if ctor throws in a placement new expression.
template <class T>
inline void operator delete(void* ptr, ocs::system::IArena& arena) throw() {
    arena.deallocate(ptr);
}

//! Placement delete[] for system::IArena.
//!
//! @note
//!  Compiler calls this if ctor throws in a placement new[] expression.
template <class T>
inline void operator delete[](void* ptr, ocs::system::IArena& arena) throw() {
    arena.deallocate(ptr);
}
