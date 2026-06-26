/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <string>

#include "ocs_core/freertos.h"
#include "ocs_core/noncopyable.h"
#include "ocs_scheduler/itask_scheduler.h"
#include "ocs_status/code.h"

namespace ocs {
namespace test {

//! Run task scheduler in the standalone FreeRTOS task.
class TaskSchedulerRunner : private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p task_scheduler - task scheduler to run.
    //!  - @p id - FreeRTOS task unique identifier.
    //!  - @p stack_size - FreeRTOS task stack size, in bytes.
    //!  - @p priority - FreeRTOS task priority.
    TaskSchedulerRunner(scheduler::ITaskScheduler& task_scheduler,
                        const char* id,
                        size_t stack_size,
                        uint8_t priority);

    //! Stop task scheduler and delete FreeRTOS task.
    ~TaskSchedulerRunner();

    //! Start task scheduler in the standalone FreeRTOS task.
    status::StatusCode start();

private:
    static void start_(void* arg);

    const std::string id_;
    const size_t stack_size_ { 0 };
    const uint8_t priority_ { 0 };

    scheduler::ITaskScheduler& task_scheduler_;

    TaskHandle_t handle_ { nullptr };
};

} // namespace test
} // namespace ocs
