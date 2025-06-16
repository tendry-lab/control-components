/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <memory>
#include <utility>
#include <vector>

#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_adc/adc_oneshot.h"

#include "ocs_core/noncopyable.h"
#include "ocs_io/adc/istore.h"
#include "ocs_io/adc/target_esp32/oneshot_reader.h"

namespace ocs {
namespace io {
namespace adc {

class OneshotStore : public IStore, private core::NonCopyable<> {
public:
    //! Initialize ADC unit.
    OneshotStore(adc_unit_t unit, adc_atten_t atten, adc_bitwidth_t bitwidth);

    //! Release ADC unit resources.
    ~OneshotStore();

    //! Configure ADC reading for @p channel.
    //!
    //! @return
    //!  A valid pointer if the ADC was configured properly.
    //!  nullptr if ADC was already configured.
    //!  nullptr if maximum number of channels were already configured.
    IStore::IReaderPtr add(Channel channel) override;

private:
    adc_oneshot_chan_cfg_t config_;
    adc_oneshot_unit_init_cfg_t unit_config_;

    adc_oneshot_unit_handle_t unit_handle_ { nullptr };

    std::vector<std::pair<Channel, IStore::IReaderPtr>> adcs_;
};

} // namespace adc
} // namespace io
} // namespace ocs
