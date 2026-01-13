/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstddef>

#include "ocs_status/code.h"
#include "ocs_system/time.h"

namespace ocs {
namespace io {
namespace i2c {

class ITransceiver {
public:
    //! Destroy.
    virtual ~ITransceiver() = default;

    //! Send data to the I2C device.
    //!
    //! @params
    //!  - @p buf - sending data, should be at least @p size bytes long.
    //!  - @p timeout - interval to wait for the operation to complete,
    //!       -1 means wait forever.
    virtual status::StatusCode
    send(const uint8_t* buf, size_t size, system::Time timeout) = 0;

    //! Receive data from the I2C device.
    //!
    //! @params
    //!  - @p buf - buffer to store received data, should be at least @p size bytes long.
    //!  - @p timeout - interval to wait for the operation to complete,
    //!       -1 means wait forever.
    virtual status::StatusCode
    receive(uint8_t* buf, size_t size, system::Time timeout) = 0;

    //! Perform a write-read transaction on the I2C bus.
    //!
    //! @params
    //!  - @p wbuf with sending data, should be at least @p wsize bytes long.
    //!  - @p rbuf to store received data, should be at least @p rsize bytes long.
    //!  - @p timeout to wait for the operation to complete, -1 means wait forever.
    virtual status::StatusCode send_receive(const uint8_t* wbuf,
                                            size_t wsize,
                                            uint8_t* rbuf,
                                            size_t rsize,
                                            system::Time timeout) = 0;
};

} // namespace i2c
} // namespace io
} // namespace ocs
