/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "ocs_core/cond.h"
#include "ocs_core/static_mutex.h"
#include "ocs_status/code.h"

namespace ocs {
namespace core {

class Future : private NonCopyable<> {
public:
    //! Initialize.
    Future();

    //! Return status of the asynchronous execution;
    status::StatusCode code() const;

    //! Notify that asynchronous execution has finished with @p code.
    status::StatusCode notify(status::StatusCode code);

    //! Wait @p wait for the asynchronous execution to be finish.
    status::StatusCode wait(TickType_t wait = portMAX_DELAY);

private:
    mutable StaticMutex mu_;
    Cond cond_;

    status::StatusCode code_ { status::StatusCode::Last };
};

} // namespace core
} // namespace ocs
