/*
 * Copyright (c) 2025, Open Control Systems authors
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

} // namespace

AnalogConfig::AnalogConfig(storage::IStorage& storage, uint16_t min, uint16_t max)
    : def_min_(min)
    , def_max_(max)
    , storage_(storage) {
    auto code = algo::StorageOps::prob_read(storage_, min_key_, &min_, sizeof(min_));
    if (code != status::StatusCode::OK) {
        if (code != status::StatusCode::NoData) {
            ocs_loge(log_tag, "failed to read min from storage: %s",
                     status::code_to_str(code));
        }

        min_ = def_min_;
    }

    code = algo::StorageOps::prob_read(storage_, max_key_, &max_, sizeof(max_));
    if (code != status::StatusCode::OK) {
        if (code != status::StatusCode::NoData) {
            ocs_loge(log_tag, "failed to read max from storage: %s",
                     status::code_to_str(code));
        }

        max_ = def_max_;
    }
}

bool AnalogConfig::valid() const {
    return min_ < max_;
}

uint16_t AnalogConfig::get_min() const {
    return min_;
}

uint16_t AnalogConfig::get_max() const {
    return max_;
}

status::StatusCode AnalogConfig::configure(uint16_t min, uint16_t max) {
    if (min >= max) {
        return status::StatusCode::InvalidArg;
    }

    bool modified = false;

    if (min_ != min) {
        const auto code = storage_.write(min_key_, &min, sizeof(min));
        if (code != status::StatusCode::OK) {
            return code;
        }

        min_ = min;
        modified = true;
    }

    if (max_ != max) {
        const auto code = storage_.write(max_key_, &max, sizeof(max));
        if (code != status::StatusCode::OK) {
            return code;
        }

        max_ = max;
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

    return code;
}

} // namespace sensor
} // namespace ocs
