/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
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
