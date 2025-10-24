/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <functional>

#include "ocs_core/noncopyable.h"
#include "ocs_fmt/json/cjson_builder.h"
#include "ocs_http/ihandler.h"
#include "ocs_http/irouter.h"
#include "ocs_sensor/ds18b20/store.h"
#include "ocs_system/isuspender.h"

namespace ocs {
namespace pipeline {
namespace httpserver {

class DS18B20Handler : private http::IHandler, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p router to register endpoints.
    //!  - @p suspender to suspend the system during sensors operations.
    //!  - @p store to perform operations on sensors.
    DS18B20Handler(http::IRouter& router,
                   system::ISuspender& suspender,
                   sensor::ds18b20::Store& store);

private:
    using HandleConfigurationFunc =
        std::function<status::StatusCode(cJSON*, sensor::ds18b20::Sensor&)>;

    static constexpr unsigned scan_response_buffer_size_ { 256 };
    static constexpr unsigned read_response_buffer_size_ { 256 };
    static constexpr unsigned write_response_buffer_size_ { 256 };
    static constexpr unsigned erase_response_buffer_size_ { 256 };

    static constexpr TickType_t scan_wait_interval_ { pdMS_TO_TICKS(10 * 1000) };
    static constexpr TickType_t read_wait_interval_ { pdMS_TO_TICKS(5 * 1000) };
    static constexpr TickType_t write_wait_interval_ { pdMS_TO_TICKS(5 * 1000) };
    static constexpr TickType_t erase_wait_interval_ { pdMS_TO_TICKS(5 * 1000) };

    status::StatusCode serve_http(http::IResponseWriter& w, http::IRequest&) override;

    status::StatusCode handle_scan_(http::IResponseWriter& w, http::IRequest& r);

    status::StatusCode scan_(cJSON* json,
                             fmt::json::CjsonUniqueBuilder& builder,
                             onewire::Bus& bus,
                             const sensor::ds18b20::Store::SensorList& sensors);

    status::StatusCode scan_rom_code_(cJSON* json, onewire::Bus& bus);

    status::StatusCode format_sensors_(cJSON* json,
                                       fmt::json::CjsonUniqueBuilder& builder,
                                       const sensor::ds18b20::Store::SensorList& sensors);

    status::StatusCode format_sensor_(cJSON* json,
                                      fmt::json::CjsonUniqueBuilder& builder,
                                      const sensor::ds18b20::Sensor& sensors);

    status::StatusCode handle_configuration_(http::IResponseWriter& w,
                                             http::IRequest& r,
                                             unsigned wait_interval,
                                             unsigned response_size,
                                             HandleConfigurationFunc func);

    status::StatusCode read_configuration_(cJSON* json, sensor::ds18b20::Sensor&);

    status::StatusCode handle_write_configuration_(http::IResponseWriter& w,
                                                   http::IRequest& r);

    status::StatusCode write_configuration_(cJSON* json,
                                            onewire::Bus& bus,
                                            sensor::ds18b20::Sensor* sensor,
                                            const std::string_view& serial_number,
                                            const std::string_view& resolution);

    status::StatusCode
    find_rom_code_(onewire::Bus& bus,
                   sensor::ds18b20::Sensor::Configuration& configuration);

    status::StatusCode erase_configuration_(cJSON* json, sensor::ds18b20::Sensor& sensor);

    status::StatusCode
    send_response_(unsigned buffer_size, cJSON* json, http::IResponseWriter& w);

    system::ISuspender& suspender_;
    sensor::ds18b20::Store& store_;
};

} // namespace httpserver
} // namespace pipeline
} // namespace ocs
