/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_sensor/analog_config_store.h"

namespace ocs {
namespace sensor {

void AnalogConfigStore::add(AnalogConfig& config) {
    configASSERT(get(config.get_id()) == nullptr);

    configs_.push_back(&config);
}

AnalogConfig* AnalogConfigStore::get(const char* id) const {
    for (auto& config : configs_) {
        if (!strcmp(id, config->get_id())) {
            return config;
        }
    }

    return nullptr;
}

const AnalogConfigStore::ConfigList& AnalogConfigStore::get_all() const {
    return configs_;
}

} // namespace sensor
} // namespace ocs
