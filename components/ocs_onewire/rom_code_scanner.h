/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_onewire/bus.h"
#include "ocs_onewire/rom_code.h"
#include "ocs_status/code.h"

namespace ocs {
namespace onewire {

class RomCodeScanner : private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p bus to scan for rom codes.
    explicit RomCodeScanner(Bus& bus);

    //! Scan rom code.
    status::StatusCode scan(RomCode& rom_code);

    //! Start the scan process from the beginning.
    void reset();

private:
    status::StatusCode scan_(uint8_t* buf, unsigned size);
    uint8_t handle_discrepancy_(const uint8_t* buf, int position);

    static constexpr unsigned bits_in_byte_ = 8;

    Bus& bus_;

    bool finished_ { false };
    int prev_discrepancy_ { -1 };
    int curr_discrepancy_ { -1 };
    unsigned conflict_count_ { 0 };
};

} // namespace onewire
} // namespace ocs
