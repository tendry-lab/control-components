/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstring>

#include "ocs_io/adc/target_esp32/curve_fitting_converter.h"

namespace ocs {
namespace io {
namespace adc {

CurveFittingConverter::CurveFittingConverter(adc_unit_t unit,
                                             adc_atten_t atten,
                                             adc_bitwidth_t bitwidth) {
    memset(&calibration_config_, 0, sizeof(calibration_config_));
    calibration_config_.unit_id = unit;
    calibration_config_.atten = atten;
    calibration_config_.bitwidth = bitwidth;

    ESP_ERROR_CHECK(
        adc_cali_create_scheme_curve_fitting(&calibration_config_, &calibration_handle_));
}

CurveFittingConverter::~CurveFittingConverter() {
    ESP_ERROR_CHECK(adc_cali_delete_scheme_curve_fitting(calibration_handle_));
}

} // namespace adc
} // namespace io
} // namespace ocs
