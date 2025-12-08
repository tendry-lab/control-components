/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_system/icrc32_calculator.h"

namespace ocs {
namespace system {

//! Little endian CRC32 calculator.
class LeCrc32Calculator : public ICrc32Calculator, private core::NonCopyable<> {
public:
    //! Calculate CRC32.
    uint32_t calculate(uint32_t crc, const uint8_t* buf, size_t len) override;
};

} // namespace system
} // namespace ocs
