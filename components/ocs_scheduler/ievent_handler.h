/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_status/code.h"

namespace ocs {
namespace scheduler {

class IEventHandler {
public:
    //! Destroy.
    virtual ~IEventHandler() = default;

    //! Handle an arbitrary event.
    virtual status::StatusCode handle_event() = 0;
};

} // namespace scheduler
} // namespace ocs
