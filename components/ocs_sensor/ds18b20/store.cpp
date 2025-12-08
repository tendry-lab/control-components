/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/log.h"
#include "ocs_io/gpio/default_gpio.h"
#include "ocs_sensor/ds18b20/resolution_to_str.h"
#include "ocs_sensor/ds18b20/store.h"
#include "ocs_status/code_to_str.h"
#include "ocs_status/macros.h"

namespace ocs {
namespace sensor {
namespace ds18b20 {

namespace {

const char* log_tag = "ds18b20_store";

} // namespace

Store::Store(system::IRtDelayer& delayer, size_t max_event_count)
    : max_event_count_(max_event_count)
    , delayer_(delayer) {
    configASSERT(max_event_count_);
}

status::StatusCode Store::run() {
    for (auto& [gpio, node] : nodes_) {
        const auto code = node->run();
        if (code != status::StatusCode::OK) {
            ocs_logw(log_tag, "failed to handle events on the bus: gpio=%d code=%s",
                     static_cast<int>(gpio), status::code_to_str(code));
        }
    }

    return status::StatusCode::OK;
}

status::StatusCode Store::add(Sensor& sensor, io::gpio::Gpio gpio, const char* gpio_id) {
    NodePtr node = get_node_(gpio);
    if (!node) {
        node = add_node_(gpio, gpio_id);
    }
    configASSERT(node);

    return node->add(sensor);
}

scheduler::AsyncFuncScheduler::FuturePtr Store::schedule(io::gpio::Gpio gpio,
                                                         Store::Func func) {
    NodePtr node = get_node_(gpio);
    if (!node) {
        return nullptr;
    }

    return node->schedule(func);
}

Store::NodePtr Store::get_node_(io::gpio::Gpio gpio) {
    for (const auto& item : nodes_) {
        if (item.first == gpio) {
            return item.second;
        }
    }

    return nullptr;
}

Store::NodePtr Store::add_node_(io::gpio::Gpio gpio, const char* gpio_id) {
    auto node =
        NodePtr(new (std::nothrow) Node(delayer_, gpio, gpio_id, max_event_count_));
    if (!node) {
        return nullptr;
    }

    nodes_.push_back(std::make_pair(gpio, node));

    return node;
}

Store::Node::Node(system::IRtDelayer& delayer,
                  io::gpio::Gpio gpio,
                  const char* gpio_id,
                  size_t max_event_count)
    : func_scheduler_(max_event_count) {
    gpio_.reset(new (std::nothrow) io::gpio::DefaultGpio(gpio_id, gpio));
    configASSERT(gpio_);

    // For timing selection, see reference:
    // https://www.analog.com/en/resources/technical-articles/1wire-communication-through-software.html
    bus_.reset(new (std::nothrow) onewire::Bus(
        delayer, *gpio_,
        onewire::Bus::Params {
            .reset_pulse_interval = system::Duration::microsecond * 480,
            .presence_pulse_interval = system::Duration::microsecond * 60,
            .write_slot_interval = system::Duration::microsecond * 60,
            .write_bit_interval = system::Duration::microsecond * 10,
            .write_recovery_interval = system::Duration::microsecond * 1,
            .read_slot_interval = system::Duration::microsecond * 60,
            .read_bit_init_interval = system::Duration::microsecond * 5,
            .read_bit_rc_interval = system::Duration::microsecond * 5,
            .read_recovery_interval = system::Duration::microsecond * 1,
        }));
    configASSERT(bus_);
}

status::StatusCode Store::Node::run() {
    return func_scheduler_.run();
}

status::StatusCode Store::Node::add(Sensor& sensor) {
    for (auto& s : sensors_) {
        if (strcmp(s->id(), sensor.id()) == 0) {
            return status::StatusCode::InvalidArg;
        }
    }

    Sensor::Configuration configuration;
    auto code = sensor.read_configuration(configuration);
    if (code != status::StatusCode::OK) {
        if (code != status::StatusCode::NoData) {
            ocs_loge(log_tag, "failed to load sensor configuration: id=%s code=%s",
                     sensor.id(), status::code_to_str(code));
        }
    } else {
        code = sensor.write_configuration(*bus_, configuration);
        if (code != status::StatusCode::OK) {
            ocs_loge(log_tag, "failed to reload sensor configuration: id=%s code=%s",
                     sensor.id(), status::code_to_str(code));
        } else {
            ocs_logi(log_tag,
                     "reload sensor configuration: id=%s "
                     "serial_number=" DS_SENSOR_SERIAL_NUMBER_STR " resolution=%s",
                     sensor.id(),
                     DS_SENSOR_SERIAL_NUMBER_TO_STR(configuration.rom_code.serial_number),
                     resolution_to_str(configuration.resolution));
        }
    }

    sensors_.push_back(&sensor);

    return status::StatusCode::OK;
}

scheduler::AsyncFuncScheduler::FuturePtr Store::Node::schedule(Func func) {
    return func_scheduler_.add([this, func]() {
        return func(*bus_, sensors_);
    });
}

} // namespace ds18b20
} // namespace sensor
} // namespace ocs
