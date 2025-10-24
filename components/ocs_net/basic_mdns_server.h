/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <string>
#include <vector>

#include "ocs_core/noncopyable.h"
#include "ocs_net/mdns_service.h"
#include "ocs_status/code.h"

namespace ocs {
namespace net {

//! mDNS server.
//!
//! @references
//!  - https://datatracker.ietf.org/doc/html/rfc2782
//!  - https://datatracker.ietf.org/doc/html/rfc6335
//!  - https://www.ietf.org/rfc/rfc6763.txt
class BasicMdnsServer : public core::NonCopyable<BasicMdnsServer> {
public:
    //! Initialize.
    BasicMdnsServer(const char* hostname);

    //! Destroy.
    virtual ~BasicMdnsServer() = default;

    //! Start mDNS server.
    virtual status::StatusCode start() = 0;

    //! Stop mDNS server.
    virtual status::StatusCode stop() = 0;

    //! Add mDNS service.
    void add(MdnsService& service);

protected:
    const std::string hostname_;

    std::vector<MdnsService*> services_;
};

} // namespace net
} // namespace ocs
