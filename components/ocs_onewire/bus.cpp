/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_algo/bit_ops.h"
#include "ocs_onewire/bus.h"
#include "ocs_status/macros.h"

namespace ocs {
namespace onewire {

Bus::Bus(system::IRtDelayer& delayer, io::gpio::IGpio& gpio, Bus::Params params)
    : params_(params)
    , delayer_(delayer)
    , gpio_(gpio) {
    configASSERT(params_.reset_pulse_interval);
    configASSERT(params_.presence_pulse_interval);

    reset_remain_time_slot_ =
        params_.reset_pulse_interval - params_.presence_pulse_interval;
    configASSERT(reset_remain_time_slot_ >= 0);

    configASSERT(params_.write_slot_interval);
    configASSERT(params_.write_bit_interval);
    configASSERT(params_.write_recovery_interval);

    write_remain_time_slot_ = params_.write_slot_interval - params_.write_bit_interval;
    configASSERT(write_remain_time_slot_ >= 0);

    configASSERT(params_.read_slot_interval);
    configASSERT(params_.read_bit_init_interval);
    configASSERT(params_.read_bit_rc_interval);
    configASSERT(params_.read_recovery_interval);

    read_remain_time_slot_ = params_.read_slot_interval - params_.read_bit_init_interval
        - params_.read_bit_rc_interval;
    configASSERT(read_remain_time_slot_ >= 0);
}

status::StatusCode Bus::reset() {
    // Start transmission.
    OCS_STATUS_RETURN_ON_ERROR(gpio_.set_direction(io::gpio::Direction::Output));

    // Hold the line.
    OCS_STATUS_RETURN_ON_ERROR(gpio_.turn_off());
    OCS_STATUS_RETURN_ON_ERROR(delayer_.delay(params_.reset_pulse_interval));

    // Release the line.
    OCS_STATUS_RETURN_ON_ERROR(gpio_.turn_on());
    OCS_STATUS_RETURN_ON_ERROR(delayer_.delay(params_.presence_pulse_interval));

    // Start receiving.
    OCS_STATUS_RETURN_ON_ERROR(gpio_.set_direction(io::gpio::Direction::Input));

    io::gpio::Level level = 0;
    OCS_STATUS_RETURN_ON_ERROR(gpio_.get_level(level));

    // Wait for the end of the time slot.
    OCS_STATUS_RETURN_ON_ERROR(delayer_.delay(reset_remain_time_slot_));

    return !level ? status::StatusCode::OK : status::StatusCode::Error;
}

status::StatusCode Bus::write_bit(uint8_t bit) {
    return bit ? write_bit_one_() : write_bit_zero_();
}

status::StatusCode Bus::read_bit(uint8_t& bit) {
    // Start transmission.
    OCS_STATUS_RETURN_ON_ERROR(gpio_.set_direction(io::gpio::Direction::Output));

    // Hold the line.
    OCS_STATUS_RETURN_ON_ERROR(gpio_.turn_off());
    OCS_STATUS_RETURN_ON_ERROR(delayer_.delay(params_.read_bit_init_interval));

    // Release the line.
    OCS_STATUS_RETURN_ON_ERROR(gpio_.turn_on());
    // Wait for the sensor to start sampling the data.
    OCS_STATUS_RETURN_ON_ERROR(delayer_.delay(params_.read_bit_rc_interval));

    // Start receiving.
    OCS_STATUS_RETURN_ON_ERROR(gpio_.set_direction(io::gpio::Direction::Input));

    OCS_STATUS_RETURN_ON_ERROR(gpio_.get_level(bit));

    // Wait for the end of the time slot.
    OCS_STATUS_RETURN_ON_ERROR(delayer_.delay(read_remain_time_slot_));

    // Recovery interval after each read slot.
    OCS_STATUS_RETURN_ON_ERROR(delayer_.delay(params_.read_recovery_interval));

    return status::StatusCode::OK;
}

status::StatusCode Bus::write_byte(uint8_t byte) {
    for (uint8_t n = 0; n < bits_in_byte_; ++n) {
        const uint8_t bit = byte % 2;
        OCS_STATUS_RETURN_ON_ERROR(write_bit(bit));

        byte /= 2;
    }

    return status::StatusCode::OK;
}

status::StatusCode Bus::read_byte(uint8_t& byte) {
    for (uint8_t n = 0; n < bits_in_byte_; ++n) {
        uint8_t bit = 0;
        OCS_STATUS_RETURN_ON_ERROR(read_bit(bit));

        if (bit) {
            byte |= algo::BitOps::mask_u8(n);
        } else {
            byte &= algo::BitOps::umask_u8(n);
        }
    }

    return status::StatusCode::OK;
}

status::StatusCode Bus::read_bytes(uint8_t* buf, size_t size) {
    OCS_STATUS_RETURN_ON_FALSE(buf, status::StatusCode::InvalidArg);
    OCS_STATUS_RETURN_ON_FALSE(size, status::StatusCode::InvalidArg);

    for (size_t n = 0; n < size; ++n) {
        OCS_STATUS_RETURN_ON_ERROR(read_byte(buf[n]));
    }

    return status::StatusCode::OK;
}

status::StatusCode Bus::write_bytes(const uint8_t* buf, size_t size) {
    OCS_STATUS_RETURN_ON_FALSE(buf, status::StatusCode::InvalidArg);
    OCS_STATUS_RETURN_ON_FALSE(size, status::StatusCode::InvalidArg);

    for (size_t n = 0; n < size; ++n) {
        OCS_STATUS_RETURN_ON_ERROR(write_byte(buf[n]));
    }

    return status::StatusCode::OK;
}

status::StatusCode Bus::write_bit_one_() {
    // Start transmission.
    OCS_STATUS_RETURN_ON_ERROR(gpio_.set_direction(io::gpio::Direction::Output));

    // Hold the line.
    OCS_STATUS_RETURN_ON_ERROR(gpio_.turn_off());
    OCS_STATUS_RETURN_ON_ERROR(delayer_.delay(params_.write_bit_interval));

    // Release the line.
    OCS_STATUS_RETURN_ON_ERROR(gpio_.turn_on());

    // Wait for the end of the time slot.
    OCS_STATUS_RETURN_ON_ERROR(delayer_.delay(write_remain_time_slot_));

    // Recovery interval after each write slot.
    OCS_STATUS_RETURN_ON_ERROR(delayer_.delay(params_.write_recovery_interval));

    return status::StatusCode::OK;
}

status::StatusCode Bus::write_bit_zero_() {
    // Start transmission.
    OCS_STATUS_RETURN_ON_ERROR(gpio_.set_direction(io::gpio::Direction::Output));

    // Hold the line.
    OCS_STATUS_RETURN_ON_ERROR(gpio_.turn_off());
    // Keep holding for the entire time slot.
    OCS_STATUS_RETURN_ON_ERROR(delayer_.delay(params_.write_slot_interval));

    // Release the line.
    OCS_STATUS_RETURN_ON_ERROR(gpio_.turn_on());

    // Recovery interval after each write slot.
    OCS_STATUS_RETURN_ON_ERROR(delayer_.delay(params_.write_recovery_interval));

    return status::StatusCode::OK;
}

} // namespace onewire
} // namespace ocs
