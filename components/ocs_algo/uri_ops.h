/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <string_view>
#include <unordered_map>

namespace ocs {
namespace algo {

struct UriOps {
    using Path = std::string_view;
    using Values = std::unordered_map<std::string_view, std::string_view>;

    //! Extract path from the @p uri.
    //!
    //! @remarks
    //!  @p uri should be valid while the returned path is being used.
    static Path parse_path(const char* uri);

    //! Extract key-value query fields from the @p uri.
    //!
    //! @remarks
    //!  @p uri should be valid while the returned values are being used.
    static Values parse_query(const char* uri);
};

} // namespace algo
} // namespace ocs
