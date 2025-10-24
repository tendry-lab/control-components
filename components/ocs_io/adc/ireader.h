/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
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
