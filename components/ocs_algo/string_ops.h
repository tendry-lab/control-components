/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string_view>
#include <vector>

namespace ocs {
namespace algo {

struct StringOps {
    using Value = std::string_view;
    using Values = std::vector<Value>;

    //! Split string into substrings.
    static Values split(char delimiter, const Value& value);
};

} // namespace algo
} // namespace ocs
