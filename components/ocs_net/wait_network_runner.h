/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "freertos/FreeRTOS.h"

#include "ocs_core/noncopyable.h"
#include "ocs_net/inetwork.h"
#include "ocs_net/inetwork_runner.h"

namespace ocs {
namespace net {

class WaitNetworkRunner : public INetworkRunner, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p network to perform actual network operations.
    //!  - @p wait - how long to wait for the network to be started.
    WaitNetworkRunner(INetwork& network, TickType_t wait = portMAX_DELAY);

    //! Wait for the network to start.
    //!
    //! @remarks
    //!  If the network fails to start, it will be automatically stopped.
    status::StatusCode start() override;

    //! Stop the network.
    status::StatusCode stop() override;

private:
    status::StatusCode start_();

    const TickType_t wait_start_interval_ { pdMS_TO_TICKS(0) };

    bool stopped_ { false };
    INetwork& network_;
};

} // namespace net
} // namespace ocs
