/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <vector>

#include "ocs_core/noncopyable.h"
#include "ocs_scheduler/itask.h"

namespace ocs {
namespace scheduler {

class FanoutTask : public ITask, private core::NonCopyable<> {
public:
    //! Propagate call to the underlying tasks.
    status::StatusCode run() override;

    //! Add task to be invoked on a run() call.
    void add(ITask& task);

private:
    std::vector<ITask*> tasks_;
};

} // namespace scheduler
} // namespace ocs
