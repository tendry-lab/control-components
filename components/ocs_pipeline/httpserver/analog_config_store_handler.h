/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_algo/uri_ops.h"
#include "ocs_core/noncopyable.h"
#include "ocs_http/ihandler.h"
#include "ocs_http/irequest.h"
#include "ocs_http/iresponse_writer.h"
#include "ocs_scheduler/async_func_scheduler.h"
#include "ocs_sensor/analog_config_store.h"
#include "ocs_system/iarena.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

class AnalogConfigStoreHandler : public http::IHandler, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p arena to perform dynamic allocations.
    //!  - @p func_scheduler to schedule asynchronous configuration updates. config should
    //!    be modified in the same context as a sensor that uses that config.
    //!  - @p store to access sensor configurations.
    AnalogConfigStoreHandler(system::IArena& arena,
                             scheduler::AsyncFuncScheduler& func_scheduler,
                             sensor::AnalogConfigStore& store);

    // Update analog sensor configuration over HTTP.
    status::StatusCode serve_http(http::IResponseWriter& w, http::IRequest&) override;

private:
    status::StatusCode handle_all_(http::IResponseWriter& w);

    status::StatusCode handle_single_(http::IResponseWriter& w,
                                      const algo::UriOps::Values& values);

    status::StatusCode handle_single_get_(http::IResponseWriter& w,
                                          const sensor::AnalogConfig& config);

    status::StatusCode send_json_(http::IResponseWriter& w, cJSON* json, size_t size);

    static constexpr TickType_t wait_timeout_ = pdMS_TO_TICKS(1000 * 6);

    system::IArena& arena_;
    scheduler::AsyncFuncScheduler& func_scheduler_;
    sensor::AnalogConfigStore& store_;
};

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
