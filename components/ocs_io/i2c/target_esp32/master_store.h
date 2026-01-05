/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "driver/gpio.h"
#include "driver/i2c_master.h"

#include "ocs_core/noncopyable.h"
#include "ocs_io/gpio/types.h"
#include "ocs_io/i2c/istore.h"

namespace ocs {
namespace io {
namespace i2c {

class MasterStore : public IStore, private core::NonCopyable<> {
public:
    struct Params {
        //! Data line.
        gpio_num_t sda { GPIO_NUM_NC };

        //! Clock line.
        gpio_num_t scl { GPIO_NUM_NC };
    };

    //! Initialize I2C master bus.
    explicit MasterStore(Params params);

    //! De-initialize I2C master bus.
    ~MasterStore();

    //! Add I2C slave device to the store.
    IStore::ITransceiverPtr
    add(const char* id, AddressLength len, Address addr, TransferSpeed speed) override;

private:
    i2c_master_bus_handle_t handle_ { nullptr };
};

} // namespace i2c
} // namespace io
} // namespace ocs
