/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstring>

#include "ocs_core/log.h"
#include "ocs_io/adc/target_esp32/basic_converter.h"

namespace ocs {
namespace io {
namespace adc {

namespace {

const char* log_tag = "adc_basic_converter";

} // namespace

status::StatusCode BasicConverter::convert(int& voltage, int raw) {
    const auto err = adc_cali_raw_to_voltage(calibration_handle_, raw, &voltage);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "adc_cali_raw_to_voltage(): err=%s", esp_err_to_name(err));

        return status::StatusCode::Error;
    }

    return status::StatusCode::OK;
}

} // namespace adc
} // namespace io
} // namespace ocs
