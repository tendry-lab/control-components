/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "ocs_scheduler/itask.h"

namespace ocs {
namespace scheduler {

class AsyncTask : public ITask {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p handle to post asynchronous events.
    //!  - @p event to post to the event group.
    AsyncTask(EventGroupHandle_t handle, EventBits_t event);

    //! Schedule an event to the event group.
    status::StatusCode run() override;

private:
    const EventBits_t event_ { 0 };

    EventGroupHandle_t handle_ { nullptr };
};

} // namespace scheduler
} // namespace ocs
