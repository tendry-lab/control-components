/*
 * Copyright (c) 2025, Tendry Lab authors
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

class AnalogSampleReader : public io::adc::IReader, private core::NonCopyable<> {
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
    // The delay interval was selected experimentally to ensure that the ADC unit has
    // enough time to stabilize the reading for each channel.
    //
    // @remarks
    // - When the low-pass RC filter is used, changing the delay interval doesn't
    //   significantly affect the overall quality of the ADC readings.
    //
    // ADC reading statistics for 2 sensors (each with low-pass RC filter):
    //  - ADC reading time: 101-117 usec (1 sample), 6.5–7.5 ms (64 samples)
    //  - Standard deviation: less than 0.2%
    static constexpr core::Time delay_interval_ { core::Duration::microsecond * 40 };

    const AnalogConfig& config_;

    io::adc::IReader& reader_;
    system::IRtDelayer& delayer_;
};

} // namespace sensor
} // namespace ocs
