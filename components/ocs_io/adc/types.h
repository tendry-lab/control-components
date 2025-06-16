/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "hal/adc_types.h"

namespace ocs {
namespace io {
namespace adc {

//! ADC channel.
using Channel = adc_channel_t;

//! ADC reading bit width.
using Bitwidth = adc_bitwidth_t;

} // namespace adc
} // namespace io
} // namespace ocs
