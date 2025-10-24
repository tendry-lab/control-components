/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOSConfig.h"

#include "esp_random.h"

#include "ocs_system/target_esp32/randomizer.h"

namespace ocs {
namespace system {

uint32_t Randomizer::random(uint32_t from, uint32_t to) {
    configASSERT(from <= to);

    const uint64_t range = static_cast<uint64_t>(to) - from + 1;

    uint64_t x = 0;
    uint64_t r = 0;

    do {
        uint32_t val = esp_random();
        x = static_cast<uint64_t>(val);
        r = x % range;
    } while (x - r > (-range));

    const uint32_t ret = from + static_cast<uint32_t>(r);

    configASSERT(ret >= from);
    configASSERT(ret <= to);

    return ret;
}

} // namespace system
} // namespace ocs
