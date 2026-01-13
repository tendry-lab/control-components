/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>

#include "ocs_core/noncopyable.h"
#include "ocs_io/gpio/types.h"
#include "ocs_scheduler/itask_scheduler.h"
#include "ocs_sensor/ds18b20/store.h"

namespace ocs {
namespace sensor {
namespace ds18b20 {

class SensorPipeline : private core::NonCopyable<> {
public:
    struct Params {
        //! 1-Wire Data pin.
        io::gpio::GpioNum data_pin { static_cast<io::gpio::GpioNum>(-1) };

        //! How often to read data from the sensor.
        //!
        //! @remarks
        //! 1-Wire communication relies on the strict timings. It's not stable in an
        //! environment with many tasks and interruptions. Each sensor reading is
        //! protected by an operation guard, which automatically suspends/resumes the
        //! FreeRTOS scheduler. Very frequent sensor readings can lead to the overall
        //! performance degradation. If the frequent sensor readings are needed it's
        //! better to disable the sensor reading protection.
        system::Time read_interval { 0 };

        //! Disable sensor reading protection.
        bool disable_operation_guard { false };
    };

    //! Initialize.
    //!
    //! @params
    //!  - @p task_scheduler to schedule periodic sensor readings.
    //!  - @p storage to persist sensor configuration.
    //!  - @p sensor_store to configure sensor.
    //!  - @p id to distinguish one sensor from another.
    SensorPipeline(scheduler::ITaskScheduler& task_scheduler,
                   storage::IStorage& storage,
                   Store& sensor_store,
                   const char* id,
                   Params params);

    //! Return the underlying sensor.
    Sensor& get_sensor();

private:
    const std::string task_id_;

    std::unique_ptr<Sensor> sensor_;
    std::unique_ptr<scheduler::ITask> guard_task_;
    scheduler::ITask* sensor_task_ { nullptr };
};

} // namespace ds18b20
} // namespace sensor
} // namespace ocs
