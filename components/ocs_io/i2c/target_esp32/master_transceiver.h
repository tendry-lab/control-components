/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>

#include "driver/i2c_master.h"

#include "ocs_core/noncopyable.h"
#include "ocs_io/i2c/itransceiver.h"
#include "ocs_io/i2c/types.h"

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
    //!  - @p bus - I2C bus.
    //!  - @p device - I2C device.
    //!  - @p address - I2C device address.
    MasterTransceiver(i2c_master_bus_handle_t bus, DevicePtr device, Address address);

    //! Send data to the I2C device.
    status::StatusCode
    send(const uint8_t* buf, size_t size, system::Time timeout) override;

    //! Receive data from the I2C device.
    status::StatusCode receive(uint8_t* buf, size_t size, system::Time timeout) override;

    //! Perform a write-read transaction on the I2C bus.
    status::StatusCode send_receive(const uint8_t* wbuf,
                                    size_t wsize,
                                    uint8_t* rbuf,
                                    size_t rsize,
                                    system::Time timeout) override;

    //! Check if the I2C device is reachable within @p timeout.
    status::StatusCode probe(system::Time timeout) override;

private:
    const Address address_ { 0 };

    i2c_master_bus_handle_t bus_ { nullptr };
    DevicePtr device_;
};

} // namespace i2c
} // namespace io
} // namespace ocs
