/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_core/time.h"
#include "ocs_io/adc/ireader.h"
#include "ocs_sensor/analog_config.h"
#include "ocs_system/irt_delayer.h"

namespace ocs {
namespace sensor {

class AnalogSampleReader : public io::adc::IReader, public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p delayer to add a delay between each ADC reading.
    //!  - @p reader to perform an actual ADC reading.
    //!  - @p config to get the number of times the ADC value should be read.
    AnalogSampleReader(system::IRtDelayer& delayer,
                       io::adc::IReader& reader,
                       const AnalogConfig& config);

    //! Return the aggregated result of the ADC reading.
    status::StatusCode read(int& raw) override;

private:
    static constexpr core::Time delay_interval_ { core::Duration::microsecond * 60 };

    const AnalogConfig& config_;

    io::adc::IReader& reader_;
    system::IRtDelayer& delayer_;
};

} // namespace sensor
} // namespace ocs
