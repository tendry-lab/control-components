/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <array>
#include <string_view>

namespace ocs {
namespace algo {

//! Custom hasher for std::string_view.
struct StringHash {
    //! Enables heterogeneous lookup.
    using is_transparent = std::true_type;

    //! Hash for std::string_view.
    std::size_t operator()(std::string_view sv) const noexcept {
        return std::hash<std::string_view>()(sv);
    }
};

//! Custom equality comparison for std::string and std::string_view.
struct StringEqual {
    //! Enables heterogeneous lookup.
    using is_transparent = std::true_type;

    //! Equality comparison for std::string_view.
    bool operator()(std::string_view lhs, std::string_view rhs) const noexcept {
        return lhs == rhs;
    }
};

//! Custom hasher for std::array.
//!
//! @notes
//!  Based on combine hash implementation in boost:
//!  https://www.boost.org/doc/libs/1_35_0/doc/html/boost/hash_combine_id241013.html
template <typename T, size_t N> struct ArrayHasher {
    size_t operator()(const std::array<T, N>& arr) const {
        size_t h = 0;

        for (const auto& e : arr) {
            h ^= std::hash<T> {}(e) + 0x9e3779b9 + (h << 6) + (h >> 2);
        }

        return h;
    }
};

} // namespace algo
} // namespace ocs
