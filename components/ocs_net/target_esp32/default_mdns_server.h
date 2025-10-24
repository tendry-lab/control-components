/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_net/basic_mdns_server.h"

namespace ocs {
namespace net {

class DefaultMdnsServer : public BasicMdnsServer, private core::NonCopyable<> {
public:
    //! Initialize.
    explicit DefaultMdnsServer(const char* hostname);

    //! Start mDNS server.
    status::StatusCode start() override;

    //! Stop mDNS server.
    status::StatusCode stop() override;
};

} // namespace net
} // namespace ocs
