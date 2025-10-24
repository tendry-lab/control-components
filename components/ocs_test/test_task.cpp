/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_test/test_task.h"
#include "ocs_core/lock_guard.h"
#include "ocs_status/macros.h"

namespace ocs {
namespace test {

TestTask::TestTask(status::StatusCode code)
    : cond_(mu_)
    , code_(code) {
}

status::StatusCode TestTask::run() {
    core::LockGuard lock(mu_);

    run_called_ = true;
    ++run_call_count_;

    cond_.broadcast();

    return code_;
}

bool TestTask::was_run_called() const {
    core::LockGuard lock(mu_);

    return run_called_;
}

unsigned TestTask::run_call_count() const {
    core::LockGuard lock(mu_);

    return run_call_count_;
}

void TestTask::reset(status::StatusCode code) {
    core::LockGuard lock(mu_);

    code_ = code;
    run_called_ = false;
    run_call_count_ = 0;
}

status::StatusCode TestTask::wait(TickType_t wait) {
    core::LockGuard lock(mu_);

    while (!run_called_) {
        OCS_STATUS_RETURN_ON_ERROR(cond_.wait(wait));
    }

    return status::StatusCode::OK;
}

} // namespace test
} // namespace ocs
