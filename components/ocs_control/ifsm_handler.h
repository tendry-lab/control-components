/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_status/code.h"

namespace ocs {
namespace control {

class IFsmHandler {
public:
    //! Destroy.
    virtual ~IFsmHandler() = default;

    //! Handle a FSM state.
    virtual status::StatusCode handle_state() = 0;

    //! Handle a FSM state transition.
    virtual status::StatusCode handle_transit() = 0;
};

} // namespace control
} // namespace ocs
