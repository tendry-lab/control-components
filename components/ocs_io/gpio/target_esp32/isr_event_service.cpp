/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "driver/gpio.h"
#include "esp_err.h"

#include "ocs_core/log.h"
#include "ocs_io/gpio/target_esp32/isr_event_service.h"

namespace ocs {
namespace io {
namespace gpio {

namespace {

const char* log_tag = "gpio_isr_event_service";

} // namespace

IsrEventService::IsrEventService(int flags)
    : flags_(flags) {
}

status::StatusCode IsrEventService::start() {
    const auto err = gpio_install_isr_service(flags_);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "gpio_install_isr_service(): %s", esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

status::StatusCode IsrEventService::stop() {
    gpio_uninstall_isr_service();

    return status::StatusCode::OK;
}

status::StatusCode IsrEventService::add(Gpio gpio, scheduler::IEventHandler& handler) {
    const auto err = gpio_isr_handler_add(gpio, handle_isr_, &handler);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "gpio_isr_handler_add(): %s", esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

void IsrEventService::handle_isr_(void* arg) {
    scheduler::IEventHandler& handler = *static_cast<scheduler::IEventHandler*>(arg);
    handler.handle_event();
}

} // namespace gpio
} // namespace io
} // namespace ocs
