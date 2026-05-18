/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <functional>

#include "ocs_core/noncopyable.h"
#include "ocs_http/ihandler.h"
#include "ocs_scheduler/async_func_scheduler.h"
#include "ocs_sensor/sht4x/isensor.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

class SHT4xHandler : private http::IHandler, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p func_scheduler to schedule asynchronous operation for the SHT4x sensor.
    //!  - @p sensor to control SHT4x sensor.
    //!  - @p heater_command - heater command to use during the sensor heating.
    SHT4xHandler(scheduler::AsyncFuncScheduler& func_scheduler,
                 sensor::sht4x::ISensor& sensor,
                 sensor::sht4x::ISensor::Command heater_command);

private:
    using HandleOperationFunc =
        std::function<status::StatusCode(sensor::sht4x::ISensor&)>;

    status::StatusCode serve_http(http::IResponseWriter& w, http::IRequest&) override;

    status::StatusCode handle_operation_(http::IResponseWriter& w,
                                         HandleOperationFunc func);

    static const TickType_t wait_op_interval_ { pdMS_TO_TICKS(5 * 1000) };

    const sensor::sht4x::ISensor::Command heater_command_ {
        sensor::sht4x::ISensor::Command::ActivateHeater_20mW_100ms
    };

    scheduler::AsyncFuncScheduler& func_scheduler_;
    sensor::sht4x::ISensor& sensor_;
};

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
