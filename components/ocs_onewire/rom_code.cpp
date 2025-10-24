/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstring>

#include "ocs_onewire/crc.h"
#include "ocs_onewire/rom_code.h"
#include "ocs_status/macros.h"

namespace ocs {
namespace onewire {

RomCode::RomCode() {
    memset(serial_number, 0, sizeof(serial_number));
}

bool RomCode::valid() const {
    const auto calculated_crc = calculate_crc(reinterpret_cast<const uint8_t*>(this),
                                              sizeof(RomCode) - sizeof(crc));
    if (calculated_crc != crc) {
        return false;
    }

    return true;
}

status::StatusCode RomCode::match(onewire::Bus& bus) const {
    OCS_STATUS_RETURN_ON_ERROR(bus.reset());

    OCS_STATUS_RETURN_ON_ERROR(bus.write_byte(static_cast<uint8_t>(Command::MatchRom)));

    OCS_STATUS_RETURN_ON_ERROR(
        bus.write_bytes(reinterpret_cast<const uint8_t*>(this), sizeof(RomCode)));

    return status::StatusCode::OK;
}

status::StatusCode RomCode::read(onewire::Bus& bus) {
    OCS_STATUS_RETURN_ON_ERROR(bus.reset());

    OCS_STATUS_RETURN_ON_ERROR(bus.write_byte(static_cast<uint8_t>(Command::ReadRom)));

    OCS_STATUS_RETURN_ON_ERROR(
        bus.read_bytes(reinterpret_cast<uint8_t*>(this), sizeof(RomCode)));

    return status::StatusCode::OK;
}

} // namespace onewire
} // namespace ocs
