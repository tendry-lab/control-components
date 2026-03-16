/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_io/gpio/types.h"
#include "ocs_io/spi/ibus.h"

namespace ocs {
namespace io {
namespace spi {

class MasterBus : public IBus, private core::NonCopyable<> {
public:
    struct Params {
        //! MOSI line.
        gpio::GpioNum mosi { static_cast<io::gpio::GpioNum>(-1) };

        //! MISO line.
        gpio::GpioNum miso { static_cast<io::gpio::GpioNum>(-1) };

        //! SCLK line.
        gpio::GpioNum sclk { static_cast<io::gpio::GpioNum>(-1) };

        //! Maximum number of bytes transferred over the SPI line in a single transaction.
        int max_transfer_size { 0 };

        //! SPI peripheral ID.
        HostID host_id { 0 };

        //! SPI DMA settings.
        spi_common_dma_t dma { SPI_DMA_DISABLED };
    };

    //! Initialize SPI master bus.
    explicit MasterBus(Params params);

    //! De-initialize SPI master bus.
    ~MasterBus();

    //! Add SPI slave device to the store.
    IBus::ITransceiverPtr add(gpio::GpioNum cs, Mode mode, TransferSpeed speed) override;

private:
    const Params params_;
};

} // namespace spi
} // namespace io
} // namespace ocs
