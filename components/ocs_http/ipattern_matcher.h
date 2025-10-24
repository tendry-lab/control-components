/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstddef>
#include <cstdint>

namespace ocs {
namespace http {

//! URI pattern matcher.
class IPatternMatcher {
public:
    //! Destroy.
    virtual ~IPatternMatcher() = default;

    //! Return true if the @p reference_pattern is matched to the @p pattern_to_match.
    //!
    //! @params
    //!  - @p reference_pattern - pattern to which the other pattern is matched.
    //!  - @p pattern_to_match - pattern being matched to the reference pattern.
    //!  - @p match_upto - how many bytes of @p pattern_to_match should be compared.
    virtual bool match_pattern(const char* reference_pattern,
                               const char* pattern_to_match,
                               size_t match_upto) = 0;
};

} // namespace http
} // namespace ocs
