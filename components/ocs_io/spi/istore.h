/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>

#include "ocs_io/gpio/types.h"
#include "ocs_io/spi/itransceiver.h"
#include "ocs_io/spi/types.h"

namespace ocs {
namespace io {
namespace spi {

class IStore {
public:
    //! SPI transceiver to communicate with SPI device.
    using ITransceiverPtr = std::unique_ptr<ITransceiver>;

    //! Destroy.
    virtual ~IStore() = default;

    //! Register a new SPI device.
    //!
    //! @params
    //!  - @p id - to distinguish one SPI device from another.
    //!  - @p cs - chip-select line for the device.
    //!  - @p speed - SPI transmission speed, in hertz.
    virtual ITransceiverPtr
    add(const char* id, gpio::GpioNum cs, Mode mode, TransferSpeed speed) = 0;
};

} // namespace spi
} // namespace io
} // namespace ocs
