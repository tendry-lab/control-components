/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "ocs_core/noncopyable.h"
#include "ocs_scheduler/async_func_scheduler.h"
#include "ocs_scheduler/itask.h"

namespace ocs {
namespace scheduler {

class AsyncFunc : public ITask, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p func_scheduler to schedule asynchronous operation.
    //!  - @p task to perform an asynchronous operation.
    AsyncFunc(AsyncFuncScheduler& func_scheduler,
              ITask& task,
              TickType_t wait_interval = portMAX_DELAY);

    //! Run task asynchronously.
    status::StatusCode run() override;

private:
    const TickType_t wait_interval_ { pdMS_TO_TICKS(0) };

    AsyncFuncScheduler& func_scheduler_;
    ITask& task_;
};

} // namespace scheduler
} // namespace ocs
