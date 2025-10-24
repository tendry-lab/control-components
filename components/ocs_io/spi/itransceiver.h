/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_status/code.h"

namespace ocs {
namespace io {
namespace spi {

class ITransceiver {
public:
    //! Destroy.
    virtual ~ITransceiver() = default;

    //! Send/receive data over SPI bus.
    //!
    //! @params
    //!  - @p send_buf - sending data, should be at least @p send_buf_size bytes long.
    //!  - @p recv_buf - receiving data, should be at least @p recv_buf_size bytes long.
    virtual status::StatusCode transceive(const uint8_t* send_buf,
                                          unsigned send_buf_size,
                                          uint8_t* recv_buf,
                                          unsigned recv_buf_size) = 0;
};

} // namespace spi
} // namespace io
} // namespace ocs
