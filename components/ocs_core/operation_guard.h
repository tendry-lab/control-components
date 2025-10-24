/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"

namespace ocs {
namespace core {

class OperationGuard : private NonCopyable<> {
public:
    //! Suspend FreeRTOS scheduler to ensure a running task won't be preempted.
    //!
    //! @remarks
    //!  After the object construction FreeRTOS API shouldn't be used, since the scheduler
    //!  will be suspended. Any use of FreeRTOS queues, semaphores will lead to the
    //!  application crash. printf() or logging functions are also prohibited, since the
    //!  implementation may use locks or other FreeRTOS primitives.
    OperationGuard();

    //! Resume FreeRTOS scheduler.
    ~OperationGuard();
};

} // namespace core
} // namespace ocs
