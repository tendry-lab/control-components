/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_status/code.h"

namespace ocs {
namespace net {

class INetworkConfig {
public:
    //! Destroy.
    virtual ~INetworkConfig() = default;

    //! Reset the network configuration.
    virtual status::StatusCode reset() = 0;
};

} // namespace net
} // namespace ocs
