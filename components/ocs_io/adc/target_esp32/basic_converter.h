/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_adc/adc_cali.h"

#include "ocs_core/noncopyable.h"
#include "ocs_io/adc/iconverter.h"

namespace ocs {
namespace io {
namespace adc {

class BasicConverter : public IConverter, public core::NonCopyable<BasicConverter> {
public:
    //! Destroy.
    virtual ~BasicConverter() = default;

    //! Convert ADC raw data to calibrated voltage.
    status::StatusCode convert(int& voltage, int raw) override;

protected:
    adc_cali_handle_t calibration_handle_ { nullptr };
};

} // namespace adc
} // namespace io
} // namespace ocs
