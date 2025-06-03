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
                           const AnalogConfig& config,
                           AnalogSensor::Params params)
    : config_(config)
    , params_(params)
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

    Data data;
    data.raw = raw;
    data.voltage = voltage;
    data.moisture = calculate_moisture_(raw);
    data.status_progress = calculate_status_progress_(raw);

    override_status_progress_(data);

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

    data.prev_status = parse_status(fsm_block_.previous_state());
    data.curr_status = parse_status(fsm_block_.current_state());
    data.prev_status_duration = fsm_block_.previous_state_duration();
    data.curr_status_duration = fsm_block_.current_state_duration();
    data.write_count = fsm_block_.write_count();

    data_.set(data);

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

    const int offset = raw - config_.get_min();
    const int status_index = offset / get_status_len_();
    const int status_pos = offset - (get_status_len_() * status_index);

    const double progress = static_cast<double>(status_pos) / get_status_len_();

    return algo::MathOps::round_floor(100 * progress, 2);
}

uint16_t AnalogSensor::get_status_len_() const {
    return (config_.get_max() - config_.get_min()) / get_status_count();
}

void AnalogSensor::override_status_progress_(AnalogSensor::Data& data) {
    if (!params_.status_progress_threshold) {
        return;
    }

    if (!fsm_block_.is_in_transit()) {
        return;
    }

    if (data.status_progress > params_.status_progress_threshold) {
        return;
    }

    const auto current_status = parse_status(fsm_block_.current_state());
    const auto next_status = parse_status(fsm_block_.next_state());

    if (current_status == SoilStatus::None || next_status == SoilStatus::Error) {
        return;
    }

    double moisture = 0;
    int progress = 0;

    if (current_status != SoilStatus::Error) {
        static constexpr double moisture_per_status =
            static_cast<double>(100) / get_status_count();

        const double moisture_used =
            moisture_per_status * static_cast<uint8_t>(current_status);

        moisture = 100 - algo::MathOps::round_floor(moisture_used, 2);
        progress = 99;
    }

    data.moisture = moisture;
    data.status_progress = progress;

    fsm_block_.set_next(fsm_block_.current_state());
}

} // namespace soil
} // namespace sensor
} // namespace ocs
