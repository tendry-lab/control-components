/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

namespace ocs {
namespace algo {

struct MathOps {
    //! Round @p value down to @p decimal_places.
    static double round_floor(double value, unsigned decimal_places);
};

} // namespace algo
} // namespace ocs
