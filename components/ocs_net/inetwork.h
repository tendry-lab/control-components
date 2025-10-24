/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
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
