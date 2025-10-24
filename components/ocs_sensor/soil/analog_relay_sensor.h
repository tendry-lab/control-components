/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "ocs_core/noncopyable.h"
#include "ocs_io/gpio/igpio.h"
#include "ocs_io/gpio/types.h"
#include "ocs_scheduler/itask.h"

namespace ocs {
namespace sensor {
namespace soil {

//! Activate the sensor only when the relay is energized.
class AnalogRelaySensor : public scheduler::ITask, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p task to perform actual sensor reading.
    //!  - @p gpio - relay GPIO.
    //!  - @p turn_on_delay_interval - how long to wait after the relay is activated.
    AnalogRelaySensor(scheduler::ITask& task,
                      io::gpio::Gpio gpio,
                      TickType_t turn_on_delay_interval);

    //! Energize the relay, run the underlying task, de-energized the relay.
    status::StatusCode run() override;

private:
    scheduler::ITask& task_;

    std::unique_ptr<io::gpio::IGpio> default_gpio_;
    std::unique_ptr<io::gpio::IGpio> delay_gpio_;

    io::gpio::IGpio* gpio_ { nullptr };
};

} // namespace soil
} // namespace sensor
} // namespace ocs
