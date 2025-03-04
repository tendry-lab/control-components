/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_algo/uri_ops.h"
#include "ocs_core/noncopyable.h"
#include "ocs_http/ihandler.h"
#include "ocs_http/irouter.h"
#include "ocs_scheduler/async_func_scheduler.h"
#include "ocs_sensor/analog_config_store.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

class AnalogConfigStoreHandler : private http::IHandler, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p func_scheduler to schedule asynchronous configuration updates. config should
    //!    be modified in the same context as a sensor that uses that config.
    //!  - @p router to register HTTP endpoints.
    //!  - @p store to access sensor configurations.
    AnalogConfigStoreHandler(scheduler::AsyncFuncScheduler& func_scheduler,
                             http::IRouter& router,
                             sensor::AnalogConfigStore& store);

private:
    status::StatusCode serve_http(http::IResponseWriter& w, http::IRequest&) override;

    status::StatusCode handle_all_(http::IResponseWriter& w);

    status::StatusCode handle_single_(http::IResponseWriter& w,
                                      const algo::UriOps::Values& values);

    status::StatusCode handle_single_get_(http::IResponseWriter& w,
                                          const sensor::AnalogConfig& config);

    static constexpr TickType_t wait_timeout_ = pdMS_TO_TICKS(1000 * 6);

    scheduler::AsyncFuncScheduler& func_scheduler_;
    sensor::AnalogConfigStore& store_;
};

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
