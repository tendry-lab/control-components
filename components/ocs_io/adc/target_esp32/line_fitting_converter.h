/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
//! https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/adc_calibration.html#adc-calibration-line-fitting-scheme
class LineFittingConverter : public BasicConverter, private core::NonCopyable<> {
public:
    //! Initialize.
    LineFittingConverter(adc_unit_t unit, adc_atten_t atten, adc_bitwidth_t bitwidth);

    //! Destroy.
    ~LineFittingConverter();

private:
    adc_cali_line_fitting_config_t calibration_config_;
};

} // namespace adc
} // namespace io
} // namespace ocs
