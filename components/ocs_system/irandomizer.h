/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstddef>
#include <cstdint>

#pragma once

namespace ocs {
namespace system {

class IRandomizer {
public:
    //! Destroy
    virtual ~IRandomizer() = default;

    //! Generate a random value in the range [from; to].
    virtual uint32_t random(uint32_t from, uint32_t to) = 0;
};

} // namespace system
} // namespace ocs
