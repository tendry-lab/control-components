/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "ocs_system/irebooter.h"

namespace ocs {
namespace system {

class AsyncRebooter : public IRebooter {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p handle to post asynchronous events.
    //!  - @p event to post to the event group.
    AsyncRebooter(EventGroupHandle_t handle, EventBits_t event);

    //! Post an event to the event group.
    status::StatusCode reboot() override;

private:
    const EventBits_t event_ { 0 };

    EventGroupHandle_t handle_ { nullptr };
};

} // namespace system
} // namespace ocs
