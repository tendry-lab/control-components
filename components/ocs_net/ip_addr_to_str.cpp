/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstring>

#include "lwip/inet.h"

#include "ocs_net/ip_addr_to_str.h"

namespace ocs {
namespace net {

ip_addr_to_str::ip_addr_to_str(ip_addr_t addr) {
    if (!inet_ntoa_r(addr, buf_, sizeof(buf_))) {
        strcpy(buf_, "<none>");
    }
}

const char* ip_addr_to_str::c_str() const {
    return buf_;
}

} // namespace net
} // namespace ocs
