/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_sensor/soil/analog_sensor.h"
#include "ocs_algo/math_ops.h"
#include "ocs_core/log.h"
#include "ocs_core/time.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace sensor {
namespace soil {

namespace {

SoilStatus parse_status(control::FsmBlock::State state) {
    if (state < static_cast<control::FsmBlock::State>(SoilStatus::None)
        && state >= static_cast<control::FsmBlock::State>(SoilStatus::Last)) {
        return SoilStatus::None;
    }

    return static_cast<SoilStatus>(state);
}

const char* log_tag = "soil_analog_sensor";

} // namespace

AnalogSensor::AnalogSensor(io::adc::IReader& reader,
                           io::adc::IConverter& converter,
                           control::FsmBlock& fsm_block,
                           const AnalogConfig& config)
    : config_(config)
    , reader_(reader)
    , converter_(converter)
    , fsm_block_(fsm_block) {
}

status::StatusCode AnalogSensor::run() {
    if (!config_.valid()) {
        return status::StatusCode::InvalidState;
    }

    int raw = 0;
    auto code = reader_.read(raw);
    if (code != status::StatusCode::OK) {
        return code;
    }

    int voltage = 0;
    code = converter_.convert(voltage, raw);
    if (code != status::StatusCode::OK) {
        return code;
    }

    fsm_block_.update();
    fsm_block_.set_next(static_cast<control::FsmBlock::State>(calculate_status_(raw)));

    if (fsm_block_.is_in_transit()) {
        const auto code = fsm_block_.transit();
        if (code != status::StatusCode::OK) {
            ocs_logw(log_tag,
                     "failed to transit block to the next state: code=%s prev_state=%u "
                     "curr_state=%u prev_dur=%lli curr_dur=%lli",
                     status::code_to_str(code), fsm_block_.previous_state(),
                     fsm_block_.current_state(), fsm_block_.previous_state_duration(),
                     fsm_block_.current_state_duration());
        }
    }

    update_data_(raw, voltage);

    return status::StatusCode::OK;
}

AnalogSensor::Data AnalogSensor::get_data() const {
    return data_.get();
}

bool AnalogSensor::is_invalid_input_(int raw) const {
    return raw < config_.get_min() || raw > config_.get_max();
}

double AnalogSensor::calculate_moisture_(int raw) const {
    if (is_invalid_input_(raw)) {
        return 0;
    }

    const int range = config_.get_max() - config_.get_min();
    const int offset = raw - config_.get_min();

    const double loss = static_cast<double>(offset) / range;
    const double remain = 1 - loss;

    return algo::MathOps::round_floor(100 * remain, 2);
}

SoilStatus AnalogSensor::calculate_status_(int raw) const {
    if (is_invalid_input_(raw)) {
        return SoilStatus::Error;
    }

    const auto offset = raw - config_.get_min();
    const auto status_index = offset / get_status_len_();

    return statuses_[status_index];
}

uint8_t AnalogSensor::calculate_status_progress_(int raw) const {
    if (is_invalid_input_(raw)) {
        return 0;
    }

    const auto offset = raw - config_.get_min();
    const auto status_index = offset / get_status_len_();
    const auto status_pos = offset - (get_status_len_() * status_index);
    const float loss = static_cast<float>(status_pos) / get_status_len_();

    return loss * 100;
}

uint16_t AnalogSensor::get_status_len_() const {
    return (config_.get_max() - config_.get_min()) / get_status_count();
}

void AnalogSensor::update_data_(int raw, int voltage) {
    Data data;

    data.raw = raw;
    data.voltage = voltage;
    data.moisture = calculate_moisture_(raw);
    data.prev_status = parse_status(fsm_block_.previous_state());
    data.curr_status = parse_status(fsm_block_.current_state());
    data.prev_status_duration = fsm_block_.previous_state_duration();
    data.curr_status_duration = fsm_block_.current_state_duration();
    data.write_count = fsm_block_.write_count();
    data.status_progress = calculate_status_progress_(raw);

    data_.set(data);
}

} // namespace soil
} // namespace sensor
} // namespace ocs
