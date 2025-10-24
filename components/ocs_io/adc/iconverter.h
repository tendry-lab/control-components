/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_status/code.h"

namespace ocs {
namespace io {
namespace adc {

class IConverter {
public:
    //! Destroy.
    virtual ~IConverter() = default;

    //! Convert ADC raw data to calibrated voltage.
    virtual status::StatusCode convert(int& voltage, int raw) = 0;
};

} // namespace adc
} // namespace io
} // namespace ocs
