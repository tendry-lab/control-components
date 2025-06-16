/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

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
