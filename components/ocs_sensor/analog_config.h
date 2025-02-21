/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstdint>

#include "ocs_core/noncopyable.h"
#include "ocs_storage/istorage.h"

namespace ocs {
namespace sensor {

class AnalogConfig : public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p storage to read analog sensor configuration.
    //!  - @p min - default minimum value for the sensor.
    //!  - @p max - default maximum value for the sensor.
    AnalogConfig(storage::IStorage& storage, uint16_t min, uint16_t max);

    //! Return true if the config is valid.
    //!
    //! @remarks
    //!  - Min value should be less than max value.
    bool valid() const;

    //! Return the minimum configured value.
    uint16_t get_min() const;

    //! Return the maximum configured value.
    uint16_t get_max() const;

    //! Configure sensor.
    //!
    //! @remarks
    //!  - Changes are applied immediately.
    status::StatusCode configure(uint16_t min, uint16_t max);

    //! Reset analog sensor configuration.
    //!
    //! @remarks
    //!  - Changes are applied immediately.
    status::StatusCode reset();

private:
    static constexpr const char* min_key_ = "min";
    static constexpr const char* max_key_ = "max";

    const uint16_t def_min_ { 0 };
    const uint16_t def_max_ { 0 };

    uint16_t min_ { 0 };
    uint16_t max_ { 0 };

    storage::IStorage& storage_;
};

} // namespace sensor
} // namespace ocs
