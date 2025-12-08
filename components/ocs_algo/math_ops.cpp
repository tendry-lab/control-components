/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cmath>

#include "ocs_algo/math_ops.h"

namespace ocs {
namespace algo {

double MathOps::round_floor(double value, uint8_t decimal_places) {
    const auto multiplier = std::pow(10.0, decimal_places);
    const auto ret = std::floor(value * multiplier) / multiplier;
    return ret;
}

} // namespace algo
} // namespace ocs
