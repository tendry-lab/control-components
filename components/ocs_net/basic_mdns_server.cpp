/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_net/basic_mdns_server.h"

namespace ocs {
namespace net {

BasicMdnsServer::BasicMdnsServer(const char* hostname)
    : hostname_(hostname) {
}

void BasicMdnsServer::add(MdnsService& service) {
    services_.push_back(&service);
}

} // namespace net
} // namespace ocs
