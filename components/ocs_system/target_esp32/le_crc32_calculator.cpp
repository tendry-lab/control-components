/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "rom/crc.h"

#include "ocs_system/target_esp32/le_crc32_calculator.h"

namespace ocs {
namespace system {

uint32_t LeCrc32Calculator::calculate(uint32_t crc, const uint8_t* buf, size_t len) {
    return crc32_le(crc, buf, len);
}

} // namespace system
} // namespace ocs
