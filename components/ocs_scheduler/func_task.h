/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <functional>

#include "ocs_core/noncopyable.h"
#include "ocs_scheduler/itask.h"
#include "ocs_status/code.h"

namespace ocs {
namespace scheduler {

//! Functional task.
class FuncTask : public ITask, private ocs::core::NonCopyable<> {
public:
    using Func = std::function<ocs::status::StatusCode(void)>;

    //! Initialize.
    //!
    //! @p func to be called on each run() call.
    explicit FuncTask(Func func);

    //! Run provided callback function.
    ocs::status::StatusCode run() override;

private:
    Func func_;
};

} // namespace scheduler
} // namespace ocs
