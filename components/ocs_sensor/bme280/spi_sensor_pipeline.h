/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_io/gpio/types.h"
#include "ocs_io/spi/istore.h"
#include "ocs_scheduler/itask_scheduler.h"
#include "ocs_sensor/bme280/itransceiver.h"
#include "ocs_sensor/bme280/sensor.h"
#include "ocs_system/time.h"

namespace ocs {
namespace sensor {
namespace bme280 {

class SpiSensorPipeline : private core::NonCopyable<> {
public:
    struct Params {
        system::Time read_interval { 0 };
        io::gpio::GpioNum cs_gpio { static_cast<io::gpio::GpioNum>(-1) };
        Sensor::Params sensor;
    };

    //! Initialize.
    //!
    //! @params
    //!  - @p task_scheduler to register a task for periodic sensor reading.
    //!  - @p store to register a new SPI device.
    SpiSensorPipeline(scheduler::ITaskScheduler& task_scheduler,
                      io::spi::IStore& store,
                      Params params);

    //! Return the underlying sensor.
    Sensor& get_sensor();

private:
    io::spi::IStore::ITransceiverPtr spi_transceiver_;
    std::unique_ptr<ITransceiver> register_transceiver_;
    std::unique_ptr<Sensor> sensor_;
};

} // namespace bme280
} // namespace sensor
} // namespace ocs
