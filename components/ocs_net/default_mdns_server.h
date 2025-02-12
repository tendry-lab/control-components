/*
 * Copyright (c) 2024, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_net/basic_mdns_server.h"

namespace ocs {
namespace net {

class DefaultMdnsServer : public BasicMdnsServer, public core::NonCopyable<> {
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
