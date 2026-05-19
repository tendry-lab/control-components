/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "ocs_scheduler/itask.h"

namespace ocs {
namespace scheduler {

class AsyncIsrTask : public ITask {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p handle to post asynchronous events.
    //!  - @p event to post to the event group.
    AsyncIsrTask(EventGroupHandle_t handle, EventBits_t event);

    //! Schedule an event to the event group from the ISR.
    status::StatusCode run() override;

private:
    const EventBits_t event_ { 0 };

    EventGroupHandle_t handle_ { nullptr };
};

} // namespace scheduler
} // namespace ocs
