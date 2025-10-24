/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_adc/adc_oneshot.h"

#include "ocs_io/adc/ireader.h"
#include "ocs_io/adc/types.h"

namespace ocs {
namespace io {
namespace adc {

class OneshotReader : public IReader {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p channel - ADC channel to read value from.
    //!  - @p unit_handle - handle to operate with ADC unit.
    OneshotReader(Channel channel, adc_oneshot_unit_handle_t unit_handle);

    //! Read raw ADC value from the configured channel.
    status::StatusCode read(int& raw) override;

private:
    Channel channel_ { ADC_CHANNEL_0 };
    adc_oneshot_unit_handle_t unit_handle_ { nullptr };
};

} // namespace adc
} // namespace io
} // namespace ocs
