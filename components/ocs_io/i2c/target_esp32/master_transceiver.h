/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>
#include <string>

#include "driver/i2c_master.h"

#include "ocs_core/noncopyable.h"
#include "ocs_io/i2c/itransceiver.h"

namespace ocs {
namespace io {
namespace i2c {

class MasterTransceiver : public ITransceiver, private core::NonCopyable<> {
public:
    using DevicePtr = std::shared_ptr<i2c_master_dev_t>;
    static DevicePtr make_device_shared(i2c_master_dev_t* device);

    //! Initialize
    //!
    //! @params
    //!  - @p device - I2C device.
    //!  - @p id to distinguish one transceiver from another.
    MasterTransceiver(DevicePtr device, const char* id);

    //! Send data to the I2C device.
    status::StatusCode
    send(const uint8_t* buf, size_t size, system::Time timeout = -1) override;

    //! Receive data from the I2C device.
    status::StatusCode
    receive(uint8_t* buf, size_t size, system::Time timeout = -1) override;

    //! Perform a write-read transaction on the I2C bus.
    status::StatusCode send_receive(const uint8_t* wbuf,
                                    size_t wsize,
                                    uint8_t* rbuf,
                                    size_t rsize,
                                    system::Time timeout) override;

private:
    const std::string id_;

    DevicePtr device_;
};

} // namespace i2c
} // namespace io
} // namespace ocs
