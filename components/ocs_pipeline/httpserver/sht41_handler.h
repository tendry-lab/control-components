/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <functional>

#include "ocs_core/noncopyable.h"
#include "ocs_http/ihandler.h"
#include "ocs_http/irouter.h"
#include "ocs_scheduler/async_func_scheduler.h"
#include "ocs_sensor/sht41/sensor.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

class SHT41Handler : private http::IHandler, private core::NonCopyable<> {
public:
    //! Initialize.
    SHT41Handler(scheduler::AsyncFuncScheduler& func_scheduler,
                 http::IRouter& router,
                 sensor::sht41::Sensor& sensor);

private:
    using HandleOperationFunc = std::function<status::StatusCode(sensor::sht41::Sensor&)>;

    status::StatusCode serve_http(http::IResponseWriter& w, http::IRequest&) override;

    status::StatusCode handle_operation_(http::IResponseWriter& w,
                                         HandleOperationFunc func);

    static const TickType_t wait_op_interval_ { pdMS_TO_TICKS(5 * 1000) };

    scheduler::AsyncFuncScheduler& func_scheduler_;
    sensor::sht41::Sensor& sensor_;
};

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
