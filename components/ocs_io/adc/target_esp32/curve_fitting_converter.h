/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_adc/adc_cali_scheme.h"

#include "ocs_core/noncopyable.h"
#include "ocs_io/adc/target_esp32/basic_converter.h"

namespace ocs {
namespace io {
namespace adc {

//! Convert ADC raw data to calibrated voltage.
//!
//! https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/adc_calibration.html
class CurveFittingConverter : public BasicConverter, private core::NonCopyable<> {
public:
    //! Initialize.
    CurveFittingConverter(adc_unit_t unit, adc_atten_t atten, adc_bitwidth_t bitwidth);

    //! Destroy.
    ~CurveFittingConverter();

private:
    adc_cali_curve_fitting_config_t calibration_config_;
};

} // namespace adc
} // namespace io
} // namespace ocs
