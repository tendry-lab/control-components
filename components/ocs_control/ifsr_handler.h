/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_status/code.h"

namespace ocs {
namespace control {

class IFsrHandler {
public:
    //! Destroy.
    virtual ~IFsrHandler() = default;

    //! Handle factory settings reset (FSR).
    virtual status::StatusCode handle_fsr() = 0;
};

} // namespace control
} // namespace ocs
