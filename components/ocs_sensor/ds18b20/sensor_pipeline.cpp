/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_sensor/ds18b20/sensor_pipeline.h"
#include "ocs_scheduler/operation_guard_task.h"

namespace ocs {
namespace sensor {
namespace ds18b20 {

SensorPipeline::SensorPipeline(scheduler::ITaskScheduler& task_scheduler,
                               storage::IStorage& storage,
                               Store& sensor_store,
                               const char* id,
                               SensorPipeline::Params params)
    : task_id_(std::string(id) + "_task") {
    sensor_.reset(new (std::nothrow) Sensor(storage, id));
    configASSERT(sensor_);

    sensor_task_ = sensor_.get();

    if (!params.disable_operation_guard) {
        guard_task_.reset(new (std::nothrow)
                              scheduler::OperationGuardTask(*sensor_task_));
        configASSERT(guard_task_);

        sensor_task_ = guard_task_.get();
    }

    configASSERT(sensor_task_);

    configASSERT(sensor_store.add(*sensor_, params.data_pin, "gpio_ds18b20_onewire")
                 == status::StatusCode::OK);

    configASSERT(task_scheduler.add(*sensor_task_, task_id_.c_str(), params.read_interval)
                 == status::StatusCode::OK);
}

Sensor& SensorPipeline::get_sensor() {
    return *sensor_;
}

} // namespace ds18b20
} // namespace sensor
} // namespace ocs
