/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_onewire/crc.h"
#include "ocs_algo/crc_ops.h"

namespace ocs {
namespace onewire {

uint8_t calculate_crc(const uint8_t* buf, unsigned size) {
    return algo::CrcOps::crc8(buf, size, 0x00, 0x8C, algo::CrcOps::BitOrder::LSB);
}

} // namespace onewire
} // namespace ocs
