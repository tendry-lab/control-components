/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>

#include "ocs_core/noncopyable.h"
#include "ocs_io/i2c/istore.h"
#include "ocs_status/code.h"

namespace ocs {
namespace io {
namespace i2c {

class MasterStorePipeline : private core::NonCopyable<> {
public:
    //! Initialize.
    explicit MasterStorePipeline(IStore::Params);

    //! Return the underlying store to register I2C devices.
    IStore& get_store();

private:
    status::StatusCode reset_();

    std::unique_ptr<IStore> store_;
    IStore::ITransceiverPtr transceiver_;
};

} // namespace i2c
} // namespace io
} // namespace ocs
