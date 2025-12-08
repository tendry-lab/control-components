/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstring>

#include "ocs_core/noncopyable.h"
#include "ocs_onewire/rom_code.h"

namespace ocs {
namespace onewire {

//! Format sensor serial number to string.
class serial_number_to_str : private core::NonCopyable<> {
public:
    static constexpr size_t str_length = strlen("AA:BB:CC:DD:EE:FF");

    //! Initialize.
    serial_number_to_str(const SerialNumber& serial_number);

    //! Return formatted serial number.
    const char* c_str() const;

private:
    char buf_[str_length + 1];
};

} // namespace onewire
} // namespace ocs
