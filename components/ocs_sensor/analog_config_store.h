/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <string>
#include <unordered_map>

#include "ocs_core/noncopyable.h"
#include "ocs_sensor/analog_config.h"

namespace ocs {
namespace sensor {

class AnalogConfigStore : public core::NonCopyable<> {
public:
    //! Add config to the store.
    void add(AnalogConfig& config, const char* id);

    //! Return the config from the store or nullptr if it doesn't exist.
    AnalogConfig* get(const char* id) const;

private:
    std::unordered_map<std::string, AnalogConfig*> configs_;
};

} // namespace sensor
} // namespace ocs
