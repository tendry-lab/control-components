/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
