/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_sensor/sht4x/sensor_pipeline.h"

namespace ocs {
namespace sensor {
namespace sht4x {

SensorPipeline::SensorPipeline(io::i2c::IStore& store,
                               scheduler::ITaskScheduler& task_scheduler,
                               storage::StorageBuilder& storage_builder,
                               const char* id,
                               SensorPipeline::Params params)
    : storage_id_(std::string(id) + "_stg")
    , transceiver_id_(std::string(id) + "_trx")
    , task_id_(std::string(id) + "_tsk") {
    storage_ = storage_builder.make(storage_id_.c_str());
    configASSERT(storage_);

    transceiver_ = store.add(transceiver_id_.c_str(), io::i2c::AddressLength::Bit_7,
                             params.i2c_addr, io::i2c::TransferSpeed::Fast);
    configASSERT(transceiver_);

    sensor_.reset(new (std::nothrow) Sensor(*transceiver_, *storage_, id, params.sensor));
    configASSERT(sensor_);

    configASSERT(task_scheduler.add(*sensor_, task_id_.c_str(), params.read_interval)
                 == status::StatusCode::OK);
}

Sensor& SensorPipeline::get_sensor() {
    return *sensor_;
}

} // namespace sht4x
} // namespace sensor
} // namespace ocs
