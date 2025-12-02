/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>
#include <string>

#include "ocs_core/noncopyable.h"
#include "ocs_io/i2c/istore.h"
#include "ocs_scheduler/itask_scheduler.h"
#include "ocs_sensor/sht4x/sensor.h"
#include "ocs_storage/storage_builder.h"
#include "ocs_system/time.h"

namespace ocs {
namespace sensor {
namespace sht4x {

class SensorPipeline : private core::NonCopyable<> {
public:
    //! Various pipeline settings.
    struct Params {
        //! Sensor I2C address.
        uint8_t i2c_addr { 0x44 };

        //! How often to read data from the sensor.
        system::Time read_interval { 0 };

        //! Various sensor settings.
        Sensor::Params sensor;
    };

    //! Initialize.
    //!
    //! @params
    //!  - @p store to register an I2C device.
    //!  - @p task_scheduler to register a task for periodic sensor reading.
    //!  - @p storage_builder to register storage for the sensor.
    //!  - @p id - unique sensor identifier, e.g. sht40 or sht41.
    //!  - @p params - various sensor settings.
    SensorPipeline(io::i2c::IStore& store,
                   scheduler::ITaskScheduler& task_scheduler,
                   storage::StorageBuilder& storage_builder,
                   const char* id,
                   Params params);

    //! Return the underlying sensor.
    Sensor& get_sensor();

private:
    const std::string storage_id_;
    const std::string transceiver_id_;
    const std::string task_id_;

    storage::StorageBuilder::IStoragePtr storage_;
    io::i2c::IStore::ITransceiverPtr transceiver_;
    std::unique_ptr<Sensor> sensor_;
};

} // namespace sht4x
} // namespace sensor
} // namespace ocs
