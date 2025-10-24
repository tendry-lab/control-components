/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "driver/i2c_master.h"

#include "ocs_core/noncopyable.h"
#include "ocs_io/gpio/types.h"
#include "ocs_io/i2c/istore.h"

namespace ocs {
namespace io {
namespace i2c {

class MasterStore : public IStore, private core::NonCopyable<> {
public:
    //! Initialize I2C master bus.
    explicit MasterStore(IStore::Params params);

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
