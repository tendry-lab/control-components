/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_net/imdns_driver.h"
#include "ocs_pipeline/config/mdns_config.h"
#include "ocs_status/code.h"

namespace ocs {
namespace pipeline {
namespace network {

//! Allow the device to be discovered by the device-hub software.
//!
//! @reference
//!   https://github.com/open-control-systems/device-hub/blob/master/docs/features.md#mdns-auto-discovery
class MdnsAutodiscoveryPipeline : public core::NonCopyable<> {
public:
    //! Initialize
    MdnsAutodiscoveryPipeline(net::IMdnsDriver& driver, config::MdnsConfig& config);

    //! Add mDNS service to be discovered by the device-hub.
    //!
    //! @params
    //!  - @p service - mDNS service type.
    //!  - @p service - mDNS service port.
    //!  - @p path - optional URI path
    //!    - http://device.local:8081
    //!    - http://device.local:8081/api/v1
    status::StatusCode
    add(net::IMdnsDriver::Service service, unsigned port, const char* path);

private:
    static constexpr const char* domain_ = "local";

    net::IMdnsDriver& driver_;
    config::MdnsConfig& config_;
};

} // namespace network
} // namespace pipeline
} // namespace ocs
