/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <vector>

#include "ocs_core/noncopyable.h"
#include "ocs_sensor/analog_config.h"

namespace ocs {
namespace sensor {

class AnalogConfigStore : private core::NonCopyable<> {
public:
    using ConfigList = std::vector<AnalogConfig*>;

    //! Add config to the store.
    void add(AnalogConfig& config);

    //! Return the config from the store or nullptr if it doesn't exist.
    AnalogConfig* get(const char* id) const;

    //! Return all registered configs.
    const ConfigList& get_all() const;

private:
    ConfigList configs_;
};

} // namespace sensor
} // namespace ocs
