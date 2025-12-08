/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/cond.h"
#include "ocs_core/noncopyable.h"
#include "ocs_core/static_mutex.h"
#include "ocs_scheduler/itask.h"

namespace ocs {
namespace test {

class TestTask : public scheduler::ITask, private core::NonCopyable<> {
public:
    explicit TestTask(status::StatusCode code);

    status::StatusCode run() override;

    bool was_run_called() const;
    size_t run_call_count() const;

    //! Reset to initial state.
    void reset(status::StatusCode code);

    //! Wait for the task to be called.
    status::StatusCode wait(TickType_t wait = portMAX_DELAY);

private:
    mutable core::StaticMutex mu_;
    core::Cond cond_;

    status::StatusCode code_ { status::StatusCode::OK };
    bool run_called_ { false };
    size_t run_call_count_ { 0 };
};

} // namespace test
} // namespace ocs
