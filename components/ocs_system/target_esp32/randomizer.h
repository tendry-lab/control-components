/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_system/irandomizer.h"

namespace ocs {
namespace system {

class Randomizer : public IRandomizer, private core::NonCopyable<> {
public:
    //! Generate a random value based on esp_random() function.
    uint32_t random(uint32_t from, uint32_t to) override;
};

} // namespace system
} // namespace ocs
