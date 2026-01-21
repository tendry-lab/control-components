/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>

#include "ocs_io/i2c/itransceiver.h"
#include "ocs_io/i2c/types.h"

namespace ocs {
namespace io {
namespace i2c {

class IBus {
public:
    //! I2C transceiver to communicate with I2C device.
    using ITransceiverPtr = std::unique_ptr<ITransceiver>;

    //! I2C general call address.
    static constexpr uint8_t bus_fanout_address = 0x00;

    //! I2C general call reset command.
    static constexpr uint8_t bus_reset_command = 0x06;

    //! Destroy.
    virtual ~IBus() = default;

    //! Add device to the bus.
    //!
    //! @params
    //!  - @p len - I2C device address length.
    //!  - @p addr - actual I2C device address.
    //!  - @p speed - how fast data will be transmitted over I2C bus.
    virtual ITransceiverPtr add(AddressLength len, Address addr, TransferSpeed speed) = 0;
};

} // namespace i2c
} // namespace io
} // namespace ocs
