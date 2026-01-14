/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_sensor/soil/analog_relay_sensor.h"
#include "ocs_core/log.h"
#include "ocs_io/gpio/target_esp32/gpio.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace sensor {
namespace soil {

namespace {

const char* log_tag = "analog_relay_sensor";

} // namespace

AnalogRelaySensor::AnalogRelaySensor(scheduler::ITask& task,
                                     io::gpio::GpioNum gpio_num,
                                     TickType_t turn_on_delay_interval)
    : task_(task) {
    gpio_.reset(new (std::nothrow) io::gpio::Gpio(gpio_num, true));
    configASSERT(gpio_);
}

status::StatusCode AnalogRelaySensor::run() {
    if (const auto c = gpio_->turn_on(); c != ocs::status::StatusCode::OK) {
        ocs_logw(log_tag, "failed to turn on GPIO: %s", status::code_to_str(c));
    }

    const auto code = task_.run();

    if (const auto c = gpio_->turn_off(); c != ocs::status::StatusCode::OK) {
        ocs_logw(log_tag, "failed to turn off GPIO: %s", status::code_to_str(c));
    }

    return code;
}

} // namespace soil
} // namespace sensor
} // namespace ocs
