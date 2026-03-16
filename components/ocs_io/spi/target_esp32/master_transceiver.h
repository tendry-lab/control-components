/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>

#include "driver/spi_master.h"

#include "ocs_core/noncopyable.h"
#include "ocs_io/spi/itransceiver.h"

namespace ocs {
namespace io {
namespace spi {

class MasterTransceiver : public ITransceiver, private core::NonCopyable<> {
public:
    using DevicePtr = std::shared_ptr<spi_device_t>;
    static DevicePtr make_device_shared(spi_device_t* device);

    //! Initialize
    //!
    //! @params
    //!  - @p device - SPI device.
    explicit MasterTransceiver(DevicePtr device);

    //! Send/receive data over SPI bus.
    status::StatusCode transceive(const uint8_t* send_buf,
                                  size_t send_buf_size,
                                  uint8_t* recv_buf,
                                  size_t recv_buf_size) override;

private:
    DevicePtr device_;
};

} // namespace spi
} // namespace io
} // namespace ocs
