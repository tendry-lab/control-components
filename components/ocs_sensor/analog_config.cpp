/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_sensor/analog_config.h"
#include "ocs_algo/storage_ops.h"
#include "ocs_core/log.h"
#include "ocs_status/code_to_str.h"

namespace ocs {
namespace sensor {

namespace {

const char* log_tag = "analog_config";

bool validate_oversampling_mode(uint8_t mode) {
    if (mode <= static_cast<uint8_t>(AnalogConfig::OversamplingMode::None)
        || mode >= static_cast<uint8_t>(AnalogConfig::OversamplingMode::Last)) {
        return false;
    }

    return true;
}

AnalogConfig::OversamplingMode parse_oversampling_mode(uint8_t count) {
    switch (count) {
    case 1:
        return AnalogConfig::OversamplingMode::Mode_1;
    case 8:
        return AnalogConfig::OversamplingMode::Mode_8;
    case 16:
        return AnalogConfig::OversamplingMode::Mode_16;
    case 32:
        return AnalogConfig::OversamplingMode::Mode_32;
    case 64:
        return AnalogConfig::OversamplingMode::Mode_64;

    default:
        break;
    }

    return AnalogConfig::OversamplingMode::None;
}

bool validate(uint16_t min,
              uint16_t max,
              uint8_t sample_count,
              io::adc::Bitwidth bitwidth) {
    if (min >= max) {
        return false;
    }

    if (parse_oversampling_mode(sample_count) == AnalogConfig::OversamplingMode::None) {
        return false;
    }

    if (bitwidth == static_cast<io::adc::Bitwidth>(0)) {
        return false;
    }

    return true;
}

} // namespace

AnalogConfig::AnalogConfig(storage::IStorage& storage,
                           uint16_t def_min,
                           uint16_t def_max,
                           io::adc::Bitwidth def_bitwidth,
                           AnalogConfig::OversamplingMode def_oversampling_mode,
                           const char* id)
    : id_(id)
    , min_key_(std::string(id) + "_min")
    , max_key_(std::string(id) + "_max")
    , oversampling_mode_key_(std::string(id) + "_ovsm")
    , def_min_(def_min)
    , def_max_(def_max)
    , def_bitwidth_(def_bitwidth)
    , def_oversampling_mode_(def_oversampling_mode)
    , storage_(storage) {
    auto code =
        algo::StorageOps::prob_read(storage_, min_key_.c_str(), &min_, sizeof(min_));
    if (code != status::StatusCode::OK) {
        if (code != status::StatusCode::NoData) {
            ocs_loge(log_tag, "failed to read min from storage: %s",
                     status::code_to_str(code));
        }

        min_ = def_min_;
    }

    code = algo::StorageOps::prob_read(storage_, max_key_.c_str(), &max_, sizeof(max_));
    if (code != status::StatusCode::OK) {
        if (code != status::StatusCode::NoData) {
            ocs_loge(log_tag, "failed to read max from storage: %s",
                     status::code_to_str(code));
        }

        max_ = def_max_;
    }

    uint8_t oversampling_mode = 0;
    code = algo::StorageOps::prob_read(storage_, oversampling_mode_key_.c_str(),
                                       &oversampling_mode, sizeof(oversampling_mode));
    if (code != status::StatusCode::OK) {
        if (code != status::StatusCode::NoData) {
            ocs_loge(log_tag, "failed to read oversampling mode from storage: %s",
                     status::code_to_str(code));
        }

        oversampling_mode_ = def_oversampling_mode_;
    } else {
        if (validate_oversampling_mode(oversampling_mode)) {
            oversampling_mode_ = static_cast<OversamplingMode>(oversampling_mode);
        } else {
            ocs_loge(log_tag, "storage contains invalid oversampling mode=%u",
                     oversampling_mode);

            oversampling_mode_ = def_oversampling_mode_;
        }
    }
}

bool AnalogConfig::valid() const {
    return validate(min_, max_, get_sample_count(), get_bitwidth());
}

const char* AnalogConfig::get_id() const {
    return id_.c_str();
}

uint16_t AnalogConfig::get_min() const {
    return min_;
}

uint16_t AnalogConfig::get_max() const {
    return max_;
}

io::adc::Bitwidth AnalogConfig::get_bitwidth() const {
    return def_bitwidth_;
}

uint8_t AnalogConfig::get_sample_count() const {
    switch (oversampling_mode_) {
    case OversamplingMode::None:
        return 0;
    case OversamplingMode::Last:
        return 0;
    case OversamplingMode::Mode_1:
        return 1;
    case OversamplingMode::Mode_8:
        return 8;
    case OversamplingMode::Mode_16:
        return 16;
    case OversamplingMode::Mode_32:
        return 32;
    case OversamplingMode::Mode_64:
        return 64;
    }

    return 0;
}

status::StatusCode
AnalogConfig::configure(uint16_t min, uint16_t max, uint8_t sample_count) {
    if (!validate(min, max, sample_count, get_bitwidth())) {
        return status::StatusCode::InvalidArg;
    }

    bool modified = false;

    if (min_ != min) {
        const auto code = storage_.write(min_key_.c_str(), &min, sizeof(min));
        if (code != status::StatusCode::OK) {
            return code;
        }

        min_ = min;
        modified = true;
    }

    if (max_ != max) {
        const auto code = storage_.write(max_key_.c_str(), &max, sizeof(max));
        if (code != status::StatusCode::OK) {
            return code;
        }

        max_ = max;
        modified = true;
    }

    const auto oversampling_mode = parse_oversampling_mode(sample_count);

    if (oversampling_mode_ != oversampling_mode) {
        const auto code = storage_.write(oversampling_mode_key_.c_str(),
                                         &oversampling_mode, sizeof(oversampling_mode));
        if (code != status::StatusCode::OK) {
            return code;
        }

        oversampling_mode_ = oversampling_mode;
        modified = true;
    }

    return modified ? status::StatusCode::OK : status::StatusCode::NotModified;
}

status::StatusCode AnalogConfig::reset() {
    auto code = storage_.erase_all();
    if (code == status::StatusCode::NoData) {
        code = status::StatusCode::NotModified;
    }

    min_ = def_min_;
    max_ = def_max_;
    oversampling_mode_ = def_oversampling_mode_;

    return code;
}

} // namespace sensor
} // namespace ocs
