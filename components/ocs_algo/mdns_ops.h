/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
    //!   https://github.com/open-control-systems/device-hub/blob/master/docs/features.md#mdns-auto-discovery
    static void
    enable_autodiscovery(net::MdnsService& service, const char* type, const char* path);
};

} // namespace algo
} // namespace ocs
