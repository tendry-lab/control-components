/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_sensor/soil/analog_relay_sensor.h"
#include "ocs_io/gpio/default_gpio.h"
#include "ocs_io/gpio/delay_gpio.h"
#include "ocs_io/gpio/gpio_guard.h"

namespace ocs {
namespace sensor {
namespace soil {

AnalogRelaySensor::AnalogRelaySensor(scheduler::ITask& task,
                                     io::gpio::Gpio gpio,
                                     TickType_t turn_on_delay_interval)
    : task_(task) {
    default_gpio_.reset(new (std::nothrow) io::gpio::DefaultGpio("relay_sensor", gpio));
    configASSERT(default_gpio_);

    delay_gpio_.reset(new (std::nothrow) io::gpio::DelayGpio(
        *default_gpio_,
        io::gpio::DelayGpio::Params {
            .flip_delay_interval = pdMS_TO_TICKS(0),
            .turn_on_delay_interval = turn_on_delay_interval,
            .turn_off_delay_interval = pdMS_TO_TICKS(0),
        }));
    configASSERT(delay_gpio_);

    gpio_ = delay_gpio_.get();
    configASSERT(gpio_);
}

status::StatusCode AnalogRelaySensor::run() {
    io::gpio::GpioGuard gpio(*gpio_);

    return task_.run();
}

} // namespace soil
} // namespace sensor
} // namespace ocs
