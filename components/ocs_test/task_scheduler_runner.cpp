/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_test/task_scheduler_runner.h"
#include "ocs_core/freertos.h"
#include "ocs_core/log.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace test {

TaskSchedulerRunner::TaskSchedulerRunner(scheduler::ITaskScheduler& task_scheduler,
                                         const char* id,
                                         size_t stack_size,
                                         uint8_t priority)
    : id_(id)
    , stack_size_(stack_size)
    , priority_(priority)
    , task_scheduler_(task_scheduler) {
}

status::StatusCode TaskSchedulerRunner::start() {
    const auto code = task_scheduler_.start();
    if (code != status::StatusCode::OK) {
        return code;
    }

    const auto ret =
        xTaskCreate(start_, id_.c_str(), stack_size_, this, priority_, &handle_);

    return ret == pdPASS ? status::StatusCode::OK : status::StatusCode::Error;
}

TaskSchedulerRunner::~TaskSchedulerRunner() {
    configASSERT(task_scheduler_.stop() == status::StatusCode::OK);

    if (handle_) {
        vTaskDelete(handle_);
    }
}

void TaskSchedulerRunner::start_(void* arg) {
    TaskSchedulerRunner& self = *static_cast<TaskSchedulerRunner*>(arg);

    while (true) {
        const auto code = self.task_scheduler_.run();
        if (code != status::StatusCode::OK) {
            ocs_loge(self.id_.c_str(), "failed to run task scheduler: %s",
                     status::code_to_str(code));
        }
    }
}

} // namespace test
} // namespace ocs
