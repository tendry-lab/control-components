/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_net/basic_mdns_server.h"

namespace ocs {
namespace net {

BasicMdnsServer::BasicMdnsServer(const char* hostname)
    : hostname_(hostname) {
}

status::StatusCode BasicMdnsServer::handle_suspend() {
    return stop();
}

status::StatusCode BasicMdnsServer::handle_resume() {
    return start();
}

void BasicMdnsServer::add(MdnsService& service) {
    services_.push_back(&service);
}

} // namespace net
} // namespace ocs
