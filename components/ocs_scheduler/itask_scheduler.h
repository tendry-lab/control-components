/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_scheduler/itask.h"
#include "ocs_status/code.h"
#include "ocs_system/time.h"

namespace ocs {
namespace scheduler {

class ITaskScheduler {
public:
    //! Destroy.
    virtual ~ITaskScheduler() = default;

    //! Return the maximum number of tasks a scheduler can handle.
    virtual size_t max_count() const = 0;

    //! Add task to be executed periodically once per interval.
    //!
    //! @params
    //!  - @p task - task to be executed periodically.
    //!  - @p id - unique task identifier.
    //!  - @p interval - task running frequency.
    virtual status::StatusCode
    add(ITask& task, const char* id, system::Time interval) = 0;

    //! Remove task by @p id.
    virtual status::StatusCode remove(const char* id) = 0;

    //! Start scheduling registered tasks.
    virtual status::StatusCode start() = 0;

    //! Stop scheduling registered tasks.
    virtual status::StatusCode stop() = 0;

    //! Run all registered tasks.
    virtual status::StatusCode run() = 0;
};

} // namespace scheduler
} // namespace ocs
