/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
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
