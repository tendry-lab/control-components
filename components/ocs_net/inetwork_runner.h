/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_status/code.h"

namespace ocs {
namespace net {

class INetworkRunner {
public:
    //! Destroy.
    virtual ~INetworkRunner() = default;

    //! Start the network.
    virtual status::StatusCode start() = 0;

    //! Stop the network.
    virtual status::StatusCode stop() = 0;
};

} // namespace net
} // namespace ocs
