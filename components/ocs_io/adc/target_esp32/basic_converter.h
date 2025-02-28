/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
