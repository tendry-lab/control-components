/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_net/mdns_service.h"

namespace ocs {
namespace algo {

struct MdnsOps {
    //! Enable mDNS auto-discovery for the provided mDNS service.
    //!
    //! @params
    //!  - @p service - mDNS service.
    //!  - @p type - device type.
    //!  - @p path - optional URI path
    //!    - http://device.local:8081
    //!    - http://device.local:8081/api/v1
    //!
    //! @reference
    //!   https://github.com/tendry-lab/device-hub/blob/master/docs/features.md#mdns-auto-discovery
    static void
    enable_autodiscovery(net::MdnsService& service, const char* type, const char* path);
};

} // namespace algo
} // namespace ocs
