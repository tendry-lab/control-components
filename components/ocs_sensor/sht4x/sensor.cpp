/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <algorithm>
#include <cstring>

#include "ocs_algo/bit_ops.h"
#include "ocs_algo/crc_ops.h"
#include "ocs_algo/math_ops.h"
#include "ocs_core/freertos.h"
#include "ocs_core/log.h"
#include "ocs_sensor/sht4x/command_to_str.h"
#include "ocs_sensor/sht4x/sensor.h"
#include "ocs_sensor/sht4x/serial_number_to_str.h"
#include "ocs_status/code_to_str.h"
#include "ocs_status/macros.h"

namespace ocs {
namespace sensor {
namespace sht4x {

namespace {

uint8_t calculate_crc(uint8_t hi, uint8_t lo) {
    const uint8_t buf[2] { hi, lo };
    return algo::CrcOps::crc8(buf, sizeof(buf), 0xFF, 0x31, algo::CrcOps::BitOrder::MSB);
}

} // namespace

Sensor::Sensor(io::i2c::ITransceiver& transceiver,
               storage::IStorage& storage,
               const char* id,
               Sensor::Params params)
    : params_(params)
    , log_tag_(id)
    , transceiver_(transceiver)
    , storage_(storage) {
    if (const auto code = read_heating_count_(); code != status::StatusCode::OK) {
        ocs_loge(log_tag_.c_str(),
                 "failed to read sensor heating count from storage: code=%s",
                 status::code_to_str(code));
    }
}

ISensor::MeasureData Sensor::get_measure_data() const {
    return measure_data_.get();
}

ISensor::HeaterData Sensor::get_heater_data() const {
    return heater_data_.get();
}

status::StatusCode Sensor::perform(ISensor::Command command) {
    switch (command) {
    case Command::MeasureHighPrecision:
    case Command::MeasureMediumPrecision:
    case Command::MeasureLowPrecision:
        return perform_measure_(command);

    case Command::SoftReset:
        return perform_reset_();

    case Command::ActivateHeater_200mW_1000ms:
    case Command::ActivateHeater_200mW_100ms:
    case Command::ActivateHeater_110mW_1000ms:
    case Command::ActivateHeater_110mW_100ms:
    case Command::ActivateHeater_20mW_1000ms:
    case Command::ActivateHeater_20mW_100ms:
        return perform_heat_(command);
    }

    configASSERT(false);
}

TickType_t Sensor::estimate_heating_delay_(Command command) {
    switch (command) {
    case Command::ActivateHeater_200mW_1000ms:
        return heating_long_pulse_delay_;

    case Command::ActivateHeater_200mW_100ms:
        return heating_short_pulse_delay_;

    case Command::ActivateHeater_110mW_1000ms:
        return heating_long_pulse_delay_;

    case Command::ActivateHeater_110mW_100ms:
        return heating_short_pulse_delay_;

    case Command::ActivateHeater_20mW_1000ms:
        return heating_long_pulse_delay_;

    case Command::ActivateHeater_20mW_100ms:
        return heating_short_pulse_delay_;

    default:
        break;
    }

    return 0;
}

status::StatusCode Sensor::perform_measure_(ISensor::Command command) {
    OCS_STATUS_RETURN_ON_ERROR(send_command_(command));

    MeasureData data;
    OCS_STATUS_RETURN_ON_ERROR(receive_data_(data));

    measure_data_.set(data);

    return status::StatusCode::OK;
}

status::StatusCode Sensor::perform_reset_() {
    ocs_logi(log_tag_.c_str(), "start resetting");

    const auto code = send_command_(Command::SoftReset);
    if (code != status::StatusCode::OK) {
        ocs_loge(log_tag_.c_str(), "reset failed: %s", status::code_to_str(code));
    } else {
        ocs_logi(log_tag_.c_str(), "reset completed");
    }

    return code;
}

status::StatusCode Sensor::perform_heat_(ISensor::Command command) {
    ocs_logi(log_tag_.c_str(), "start heating");

    if (const auto code = send_command_(command); code != ocs::status::StatusCode::OK) {
        ocs_loge(log_tag_.c_str(), "heating failed: unable to send command: %s",
                 command_to_str(command));

        return code;
    }

    const auto heating_delay = estimate_heating_delay_(command);
    configASSERT(heating_delay);

    vTaskDelay(heating_delay);

    MeasureData data;
    OCS_STATUS_RETURN_ON_ERROR(receive_data_(data));

    measure_data_.set(data);

    ++heating_count_;

    if (const auto code = write_heating_count_(); code != status::StatusCode::OK) {
        ocs_logw(log_tag_.c_str(), "failed to persist sensor heating count: %s",
                 status::code_to_str(code));
    }

    HeaterData heater_data;
    heater_data.heating_count = heating_count_;

    heater_data_.set(heater_data);

    ocs_logi(log_tag_.c_str(), "heating completed");

    return status::StatusCode::OK;
}

status::StatusCode Sensor::receive_data_(Sensor::MeasureData& data) {
    uint8_t buf[6];
    memset(buf, 0, sizeof(buf));
    OCS_STATUS_RETURN_ON_ERROR(
        transceiver_.receive(buf, sizeof(buf), params_.i2c_wait_timeout));

    const uint16_t temperature_ticks = algo::BitOps::pack_u8(buf[0], buf[1]);
    const uint8_t temperature_checksum = buf[2];
    const uint8_t temperature_checksum_calculated = calculate_crc(buf[0], buf[1]);
    if (temperature_checksum != temperature_checksum_calculated) {
        ocs_loge(log_tag_.c_str(),
                 "failed to read temperature: checksum mismatch: want=%u got=%u",
                 temperature_checksum, temperature_checksum_calculated);

        return status::StatusCode::Error;
    }

    const uint16_t humidity_ticks = algo::BitOps::pack_u8(buf[3], buf[4]);
    const uint8_t humidity_checksum = buf[5];
    const uint8_t humidity_checksum_calculcated = calculate_crc(buf[3], buf[4]);
    if (humidity_checksum != humidity_checksum_calculcated) {
        ocs_loge(log_tag_.c_str(),
                 "failed to read humidity: checksum mismatch: want=%u got=%u",
                 humidity_checksum, humidity_checksum_calculcated);

        return status::StatusCode::Error;
    }

    data.temperature = -45 + (175.0 * temperature_ticks / UINT16_MAX);
    data.temperature = algo::MathOps::round_floor(data.temperature, 2);

    data.humidity = -6 + (125.0 * humidity_ticks / UINT16_MAX);
    data.humidity = algo::MathOps::round_floor(data.humidity, 2);
    data.humidity = std::clamp(data.humidity, 0.0f, 100.0f);

    return status::StatusCode::OK;
}

status::StatusCode Sensor::send_command_(Sensor::Command command) {
    OCS_STATUS_RETURN_ON_ERROR(
        transceiver_.send(reinterpret_cast<const uint8_t*>(&command), sizeof(command),
                          params_.i2c_wait_timeout));

    vTaskDelay(params_.i2c_delay_interval);

    return status::StatusCode::OK;
}

status::StatusCode Sensor::read_heating_count_() {
    const auto code =
        storage_.read(heating_count_key_, &heating_count_, sizeof(heating_count_));
    if (code != status::StatusCode::OK && code != status::StatusCode::NoData) {
        return code;
    }

    return status::StatusCode::OK;
}

status::StatusCode Sensor::write_heating_count_() {
    const auto code =
        storage_.write(heating_count_key_, &heating_count_, sizeof(heating_count_));
    if (code != status::StatusCode::OK) {
        return code;
    }

    return status::StatusCode::OK;
}

} // namespace sht4x
} // namespace sensor
} // namespace ocs
