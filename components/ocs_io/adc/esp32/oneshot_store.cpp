/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_core/log.h"
#include "ocs_io/adc/esp32/oneshot_store.h"

namespace ocs {
namespace io {
namespace adc {

namespace {

const char* log_tag = "adc_oneshot_store";

} // namespace

OneshotStore::OneshotStore(adc_unit_t unit, adc_atten_t atten, adc_bitwidth_t bitwidth) {
    // Unit configuration.
    memset(&unit_config_, 0, sizeof(unit_config_));
    unit_config_.unit_id = unit;
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&unit_config_, &unit_handle_));

    // ADC configuration.
    memset(&config_, 0, sizeof(config_));
    config_.bitwidth = bitwidth;
    config_.atten = atten;
}

OneshotStore::~OneshotStore() {
    ESP_ERROR_CHECK(adc_oneshot_del_unit(unit_handle_));
}

IStore::IReaderPtr OneshotStore::add(Channel channel) {
    for (const auto& adc : adcs_) {
        if (adc.first == channel) {
            ocs_loge(log_tag, "channel %u already configured", channel);
            return nullptr;
        }
    }

    const auto err = adc_oneshot_config_channel(unit_handle_, channel, &config_);
    if (err != ESP_OK) {
        ocs_loge(log_tag, "adc_oneshot_config_channel(): channel=%u err=%s", channel,
                 esp_err_to_name(err));

        return nullptr;
    }

    IStore::IReaderPtr adc(new (std::nothrow) OneshotReader(channel, unit_handle_));
    configASSERT(adc);

    adcs_.emplace_back(std::pair<Channel, IStore::IReaderPtr>(channel, adc));

    return adc;
}

} // namespace adc
} // namespace io
} // namespace ocs
