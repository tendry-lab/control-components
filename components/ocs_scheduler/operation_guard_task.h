/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_scheduler/itask.h"

namespace ocs {
namespace scheduler {

class OperationGuardTask : public ITask, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p task which operations shouldn't be preempted.
    //!
    //! @remarks
    //!  Task shouldn't use any FreeRTOS primitives, for more details
    //!  see core::OperationGuard.
    explicit OperationGuardTask(ITask& task);

    //! Ensure the underlying task won't be preempted.
    status::StatusCode run() override;

private:
    ITask& task_;
};

} // namespace scheduler
} // namespace ocs
