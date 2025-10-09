/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_net/inetwork_runner.h"
#include "ocs_storage/iconfig.h"
#include "ocs_system/irebooter.h"

namespace ocs {
namespace net {

class ResetNetworkRunner : public INetworkRunner, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p runner to perform start/stop of the network.
    //!  - @p config to reset the network configuration.
    //!  - @p rebooter to reboot the system if the configuration was reset.
    ResetNetworkRunner(INetworkRunner& runner,
                       storage::IConfig& config,
                       system::IRebooter& rebooter);

    //! If the network fails to start, the network configuration is reset.
    status::StatusCode start() override;

    //! Stop the network.
    status::StatusCode stop() override;

private:
    void reset_();

    INetworkRunner& runner_;
    storage::IConfig& config_;
    system::IRebooter& rebooter_;
};

} // namespace net
} // namespace ocs
