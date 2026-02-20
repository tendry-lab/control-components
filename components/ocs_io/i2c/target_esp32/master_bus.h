/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "driver/i2c_master.h"

#include "ocs_core/noncopyable.h"
#include "ocs_io/i2c/ibus.h"

namespace ocs {
namespace io {
namespace i2c {

class MasterBus : public IBus, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p handle - I2C bus handle.
    explicit MasterBus(i2c_master_bus_handle_t handle);

    //! Add I2C device to the bus.
    ITransceiverPtr add(AddressLength len, Address addr, TransferSpeed speed) override;

private:
    i2c_master_bus_handle_t handle_ { nullptr };
};

} // namespace i2c
} // namespace io
} // namespace ocs
