/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "freertos/FreeRTOS.h"

#include "ocs_core/noncopyable.h"
#include "ocs_system/ireboot_handler.h"
#include "ocs_system/irebooter.h"

namespace ocs {
namespace system {

class Rebooter : public IRebooter, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p handler to be notified before the actual reboot is happened.
    //!  - @p delay - optional delay before reboot.
    Rebooter(IRebootHandler& handler, TickType_t delay);

    //! Restart the board.
    status::StatusCode reboot() override;

private:
    const TickType_t delay_ { 0 };

    IRebootHandler& handler_;
};

} // namespace system
} // namespace ocs
