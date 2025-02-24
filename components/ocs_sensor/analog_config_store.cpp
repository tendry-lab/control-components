/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <algorithm>
#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_sensor/analog_config_store.h"

namespace ocs {
namespace sensor {

void AnalogConfigStore::add(AnalogConfig& config) {
    configASSERT(std::find_if(configs_.begin(), configs_.end(),
                              [&config](const auto& c) {
                                  return !strcmp(c->get_id(), config.get_id());
                              })
                 == configs_.end());

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
