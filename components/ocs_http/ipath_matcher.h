/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstddef>
#include <cstdint>

namespace ocs {
namespace http {

//! URI path matcher.
class IPathMatcher {
public:
    //! Destroy.
    virtual ~IPathMatcher() = default;

    //! Return true if the @p reference_path is matched to the @p path_to_match.
    //!
    //! @params
    //!  - @p reference_path - URI path to which the other URI path is matched.
    //!  - @p path_to_match - URI path being matched to the reference URI path.
    //!  - @p match_upto - how many bytes of @p path_to_match should be compared.
    virtual bool match_path(const char* reference_path,
                            const char* path_to_match,
                            size_t match_upto) = 0;
};

} // namespace http
} // namespace ocs
