/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_io/gpio/types.h"
#include "ocs_io/spi/istore.h"

namespace ocs {
namespace io {
namespace spi {

class MasterStore : public IStore, private core::NonCopyable<> {
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
    };

    //! Initialize SPI master bus.
    explicit MasterStore(Params params);

    //! De-initialize SPI master bus.
    ~MasterStore();

    //! Add SPI slave device to the store.
    IStore::ITransceiverPtr
    add(const char* id, gpio::GpioNum cs, Mode mode, TransferSpeed speed) override;

private:
    const Params params_;
};

} // namespace spi
} // namespace io
} // namespace ocs
