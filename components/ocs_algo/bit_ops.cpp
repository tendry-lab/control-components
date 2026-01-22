/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_algo/bit_ops.h"

namespace ocs {
namespace algo {

uint32_t BitOps::mask_u32(uint8_t pos) {
    return (1UL << pos);
}

uint32_t BitOps::umask_u32(uint8_t pos) {
    return ~(mask_u32(pos));
}

uint8_t BitOps::mask_u8(uint8_t pos) {
    return (1U << pos);
}

uint8_t BitOps::umask_u8(uint8_t pos) {
    return ~(mask_u8(pos));
}

uint8_t BitOps::nth_u32(uint32_t value, uint8_t pos) {
    return mask_u32(pos) & value ? 1 : 0;
}

uint8_t BitOps::nth_u8(uint8_t value, uint8_t pos) {
    return mask_u8(pos) & value ? 1 : 0;
}

uint16_t BitOps::pack_u8(uint8_t hi, uint8_t lo) {
    return static_cast<uint16_t>(hi) << 8 | lo;
}

uint32_t BitOps::pack_u16(uint16_t hi, uint16_t lo) {
    return static_cast<uint32_t>(hi) << 16 | lo;
}

void BitOps::unpack_u8(uint16_t value, uint8_t& hi, uint8_t& lo) {
    hi = (value & 0xFF00) >> 8;
    lo = value & 0x00FF;
}

} // namespace algo
} // namespace ocs
