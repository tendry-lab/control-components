/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "ocs_core/operation_guard.h"

namespace ocs {
namespace core {

OperationGuard::OperationGuard() {
    vTaskSuspendAll();
}

OperationGuard::~OperationGuard() {
    if (xTaskResumeAll() == pdFALSE) {
        taskYIELD();
    }
}

} // namespace core
} // namespace ocs
