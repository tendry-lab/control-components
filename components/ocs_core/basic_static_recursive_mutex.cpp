/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/basic_static_recursive_mutex.h"

namespace ocs {
namespace core {

BasicStaticRecursiveMutex::BasicStaticRecursiveMutex() {
    memset(&buff_, 0, sizeof(buff_));

    sem_ = xSemaphoreCreateRecursiveMutexStatic(&buff_);
    configASSERT(sem_);
}

} // namespace core
} // namespace ocs
