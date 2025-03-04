/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_system/irandomizer.h"

namespace ocs {
namespace system {

class Randomizer : public IRandomizer, public core::NonCopyable<> {
public:
    //! Generate a random value based on esp_random() function.
    uint32_t random(uint32_t from, uint32_t to) override;
};

} // namespace system
} // namespace ocs
