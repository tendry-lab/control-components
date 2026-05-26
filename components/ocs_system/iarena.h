/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>

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

    //! Destroy object and deallocate its memory.
    template <class T> void destroy(T& object) {
        object.~T();
        deallocate(&object);
    }
};

//! Object deleter for smart pointers.
class ArenaDeleter {
public:
    ArenaDeleter()
        : arena_(nullptr) {
    }

    explicit ArenaDeleter(IArena& arena)
        : arena_(&arena) {
    }

    template <typename T> void operator()(T* t) {
        if (t && arena_) {
            if constexpr (std::is_trivially_destructible_v<T>) {
                arena_->deallocate(t);
            } else {
                arena_->destroy(*t);
            }
        }
    }

private:
    IArena* arena_ { nullptr };
};

template <typename SizeType> constexpr size_t extract_array_size(SizeType&& size) {
    return static_cast<size_t>(size);
}

//! Arena-aware smart pointer.
template <typename Ptr, typename T, typename... Args>
Ptr make_smart_ptr(IArena& arena, Args&&... args) {
    if constexpr (std::is_array_v<T>) {
        static_assert(std::is_array_v<T>, "T must be an array type");
        static_assert(std::extent_v<T> == 0,
                      "Fixed-size arrays (e.g. T[10]) are not supported");

        using ElementType = std::remove_extent_t<T>;

        static_assert(std::is_trivially_destructible_v<ElementType>,
                      "Arrays of non-trivially destructible types are not supported");

        static_assert(sizeof...(Args) == 1,
                      "Array allocation requires exactly one size argument");

        const size_t size = extract_array_size(std::forward<Args>(args)...);

        // Allocated memory will be zero-initialized.
        return Ptr(new (arena) ElementType[size](), ArenaDeleter(arena));
    } else {
        return Ptr(new (arena) T(std::forward<Args>(args)...), ArenaDeleter(arena));
    }
}

//! Wrapper around std::unique_ptr that properly destroys the object.
template <typename T> using UniquePtr = std::unique_ptr<T, ArenaDeleter>;

//! Similar to std::make_shared, but arena-aware and without exceptions.
template <typename T, typename... Args>
std::shared_ptr<T> make_shared_ptr(IArena& arena, Args&&... args) {
    return make_smart_ptr<std::shared_ptr<T>, T>(arena, std::forward<Args>(args)...);
}

//! Similar to std::make_unique, but arena-aware and without exceptions.
template <typename T, typename... Args>
UniquePtr<T> make_unique_ptr(IArena& arena, Args&&... args) {
    return make_smart_ptr<UniquePtr<T>, T>(arena, std::forward<Args>(args)...);
}

} // namespace system
} // namespace ocs

//! Placement new for system::IArena.
//!
//! @notes
//!  noexcept forces compiler to check for nullptr before calling object constructor.
inline void* operator new(size_t size, ocs::system::IArena& arena) noexcept {
    return arena.allocate(size);
}

//! Placement new[] for system::IArena.
//!
//! @notes
//!  noexcept forces compiler to check for nullptr before calling object constructor.
inline void* operator new[](size_t size, ocs::system::IArena& arena) noexcept {
    return arena.allocate(size);
}

//! Placement delete for system::IArena.
//!
//! @notes
//!  Compiler calls this if ctor throws in a placement new expression.
//!
//! @remarks
//!  Although exceptions aren't used, keep it to avoid possible compilation errors.
inline void operator delete(void* ptr, ocs::system::IArena& arena) throw() {
    arena.deallocate(ptr);
}

//! Placement delete[] for system::IArena.
//!
//! @notes
//!  Compiler calls this if ctor throws in a placement new[] expression.
//!
//! @remarks
//!  Although exceptions aren't used, keep it to avoid possible compilation errors.
inline void operator delete[](void* ptr, ocs::system::IArena& arena) throw() {
    arena.deallocate(ptr);
}
