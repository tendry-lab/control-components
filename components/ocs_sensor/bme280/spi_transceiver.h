/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_io/spi/itransceiver.h"
#include "ocs_sensor/bme280/itransceiver.h"

namespace ocs {
namespace sensor {
namespace bme280 {

class SpiTransceiver : public ITransceiver, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p transceiver to communicate with SPI device.
    explicit SpiTransceiver(io::spi::ITransceiver& transceiver);

    //! Send registers to SPI device.
    status::StatusCode
    send(const uint8_t* buf, size_t size, RegisterAddress addr) override;

    //! Receive registers from SPI device.
    status::StatusCode receive(uint8_t* buf, size_t size, RegisterAddress addr) override;

private:
    io::spi::ITransceiver& transceiver_;
};

} // namespace bme280
} // namespace sensor
} // namespace ocs
