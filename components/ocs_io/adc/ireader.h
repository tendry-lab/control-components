/*
 * Copyright (c) 2024, Tendry Lab authors
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

class IReader {
public:
    //! Destroy.
    virtual ~IReader() = default;

    //! Read raw ADC value.
    virtual status::StatusCode read(int& raw) = 0;
};

} // namespace adc
} // namespace io
} // namespace ocs
