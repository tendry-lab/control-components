/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <cstdint>
#include <string>

#include "ocs_core/noncopyable.h"
#include "ocs_io/adc/types.h"
#include "ocs_storage/istorage.h"

namespace ocs {
namespace sensor {

class AnalogConfig : public core::NonCopyable<> {
public:
    //! Number of times a value is measured before available to be read.
    //! By default a value is measured once.
    //!
    //! @remarks
    //!  More number of the measurements, more power is consumed.
    enum class OversamplingMode : uint8_t {
        None,
        Mode_1,
        Mode_8,
        Mode_16,
        Mode_32,
        Mode_64,
        Last,
    };

    //! Initialize.
    //!
    //! @params
    //!  - @p storage to read analog sensor configuration.
    //!  - @p def_min - default minimum value for the sensor.
    //!  - @p def_max - default maximum value for the sensor.
    //!  - @p def_bitwidth - default number of ADC conversion result bits.
    //!  - @p def_oversampling_mode - default oversampling mode. See @OversamplingMode.
    //!  - @p id - unique config identifier.
    AnalogConfig(storage::IStorage& storage,
                 uint16_t def_min,
                 uint16_t def_max,
                 io::adc::Bitwidth def_bitwidth,
                 OversamplingMode def_oversampling_mode,
                 const char* id);

    //! Return true if the config is valid.
    //!
    //! @remarks
    //!  - Min value should be less than max value.
    bool valid() const;

    //! Return unique config identifier.
    const char* get_id() const;

    //! Return the minimum configured value.
    uint16_t get_min() const;

    //! Return the maximum configured value.
    uint16_t get_max() const;

    //! Return the number of ADC conversion result bits.
    io::adc::Bitwidth get_bitwidth() const;

    //! Return the number of times a value should be measured.
    uint8_t get_sample_count() const;

    //! Configure sensor.
    //!
    //! @remarks
    //!  - Changes are applied immediately.
    //!  - @p min should be less than @p max.
    //!  - @p sample_count can be 1, 8, 16, 32, 64. See @OversamplingMode.
    status::StatusCode configure(uint16_t min, uint16_t max, uint8_t sample_count);

    //! Reset analog sensor configuration.
    //!
    //! @remarks
    //!  - Changes are applied immediately.
    status::StatusCode reset();

private:
    const std::string id_;
    const std::string min_key_;
    const std::string max_key_;
    const std::string oversampling_mode_key_;

    const uint16_t def_min_ { 0 };
    const uint16_t def_max_ { 0 };
    const io::adc::Bitwidth def_bitwidth_ { static_cast<io::adc::Bitwidth>(0) };
    const OversamplingMode def_oversampling_mode_ { OversamplingMode::Mode_1 };

    uint16_t min_ { 0 };
    uint16_t max_ { 0 };
    OversamplingMode oversampling_mode_ { OversamplingMode::None };

    storage::IStorage& storage_;
};

} // namespace sensor
} // namespace ocs
