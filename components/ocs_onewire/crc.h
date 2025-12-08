/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstddef>
#include <cstdint>

namespace ocs {
namespace onewire {

//! Calculate CRC-8 for Dallas Semicondactor devices.
//!
//! @remarks
//!  @p buf should be at least @p size bytes long.
//!
//! @references
//!  https://stackoverflow.com/questions/29214301/ios-how-to-calculate-crc-8-dallas-maxim-of-nsdata
//!  https://stackoverflow.com/questions/74775595/maxim-dow-crc-algorithm-cannot-re-create-example-in-application-note
uint8_t calculate_crc(const uint8_t* buf, size_t size);

} // namespace onewire
} // namespace ocs
