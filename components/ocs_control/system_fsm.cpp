/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_control/system_fsm.h"

#define EVENT_BITS_BUTTON_PRESSED BIT0
#define EVENT_BITS_ALL (EVENT_BITS_BUTTON_PRESSED)

namespace ocs {
namespace control {

SystemFsm::SystemFsm(system::IRebooter& rebooter, core::Time release_interval)
    : release_interval_(release_interval)
    , rebooter_(rebooter) {
}

status::StatusCode SystemFsm::run() {
    const EventBits_t bits =
        xEventGroupWaitBits(event_group_.get(), EVENT_BITS_ALL, pdTRUE, pdFALSE, 0);

    if (bits & EVENT_BITS_BUTTON_PRESSED) {
        rebooter_.reboot();
    }

    return status::StatusCode::OK;
}

status::StatusCode SystemFsm::handle_pressed(core::Time duration) {
    if (duration > release_interval_) {
        return status::StatusCode::Error;
    }

    BaseType_t task_woken = pdFALSE;

    xEventGroupSetBitsFromISR(event_group_.get(), EVENT_BITS_BUTTON_PRESSED, &task_woken);
    if (task_woken) {
        portYIELD_FROM_ISR();
    }

    return status::StatusCode::OK;
}

} // namespace control
} // namespace ocs
