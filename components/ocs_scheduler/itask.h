/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_status/code.h"

namespace ocs {
namespace scheduler {

class ITask {
public:
    //! Destroy.
    virtual ~ITask() = default;

    //! Run a task.
    virtual status::StatusCode run() = 0;
};

} // namespace scheduler
} // namespace ocs
