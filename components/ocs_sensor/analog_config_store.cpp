/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_sensor/analog_config_store.h"

namespace ocs {
namespace sensor {

void AnalogConfigStore::add(AnalogConfig& config, const char* id) {
    configASSERT(configs_.find(id) == configs_.end());

    configs_[id] = &config;
}

AnalogConfig* AnalogConfigStore::get(const char* id) const {
    auto it = configs_.find(id);
    if (it == configs_.end()) {
        return nullptr;
    }

    return it->second;
}

} // namespace sensor
} // namespace ocs
