/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

namespace ocs {
namespace net {

class INetworkHandler {
public:
    //! Destroy.
    virtual ~INetworkHandler() = default;

    //! Network is connected.
    virtual void handle_connect() = 0;

    //! Network is disconnected.
    virtual void handle_disconnect() = 0;
};

} // namespace net
} // namespace ocs
