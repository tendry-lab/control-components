/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_net/fanout_network_handler.h"

namespace ocs {
namespace net {

void FanoutNetworkHandler::handle_connect() {
    for (auto& handler : handlers_) {
        handler->handle_connect();
    }
}

void FanoutNetworkHandler::handle_disconnect() {
    for (auto& handler : handlers_) {
        handler->handle_disconnect();
    }
}

void FanoutNetworkHandler::add(INetworkHandler& handler) {
    handlers_.push_back(&handler);
}

} // namespace net
} // namespace ocs
