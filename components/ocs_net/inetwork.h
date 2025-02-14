/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "freertos/FreeRTOS.h"

#include "ocs_status/code.h"

namespace ocs {
namespace net {

class INetwork {
public:
    //! Destroy.
    virtual ~INetwork() = default;

    //! Start the WiFi connection process.
    virtual status::StatusCode start() = 0;

    //! Stop the WiFi connection process.
    virtual status::StatusCode stop() = 0;

    //! Wait for the WiFi connection to be established.
    virtual status::StatusCode wait(TickType_t wait) = 0;
};

} // namespace net
} // namespace ocs
