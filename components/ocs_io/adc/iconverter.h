/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
