/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_sensor/sht41/sensor_pipeline.h"

namespace ocs {
namespace sensor {
namespace sht41 {

SensorPipeline::SensorPipeline(io::i2c::IStore& store,
                               scheduler::ITaskScheduler& task_scheduler,
                               storage::StorageBuilder& storage_builder,
                               SensorPipeline::Params params) {
    storage_ = storage_builder.make("sensor_sht41");
    configASSERT(storage_);

    transceiver_ = store.add("sht41", io::i2c::IStore::AddressLength::Bit_7, 0x44,
                             io::i2c::IStore::TransferSpeed::Fast);
    configASSERT(transceiver_);

    sensor_.reset(new (std::nothrow)
                      Sensor(*transceiver_, *storage_,
                             Sensor::Params {
                                 .send_wait_interval = pdMS_TO_TICKS(20),
                                 .bus_wait_interval = system::Duration::second * 5,
                                 .measure_command = params.measure_command,
                                 .heating_command = params.heating_command,
                             }));
    configASSERT(sensor_);

    configASSERT(task_scheduler.add(*sensor_, "sht41_task", params.read_interval)
                 == status::StatusCode::OK);
}

Sensor& SensorPipeline::get_sensor() {
    return *sensor_;
}

} // namespace sht41
} // namespace sensor
} // namespace ocs
