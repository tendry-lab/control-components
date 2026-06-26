/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>

#include "ocs_core/freertos.h"
#include "ocs_core/noncopyable.h"
#include "ocs_io/gpio/igpio.h"
#include "ocs_io/gpio/types.h"
#include "ocs_scheduler/itask.h"
#include "ocs_system/iarena.h"

namespace ocs {
namespace sensor {
namespace soil {

//! Activate the sensor only when the relay is energized.
class AnalogRelaySensor : public scheduler::ITask, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p arena to perform dynamic allocations.
    //!  - @p task to perform actual sensor reading.
    //!  - @p gpio_num - relay GPIO number.
    //!  - @p turn_on_delay_interval - how long to wait after the relay is activated.
    AnalogRelaySensor(system::IArena& arena,
                      scheduler::ITask& task,
                      io::gpio::GpioNum gpio_num,
                      TickType_t turn_on_delay_interval);

    //! Energize the relay, run the underlying task, de-energized the relay.
    status::StatusCode run() override;

private:
    scheduler::ITask& task_;

    system::UniquePtr<io::gpio::IGpio> gpio_;
};

} // namespace soil
} // namespace sensor
} // namespace ocs
