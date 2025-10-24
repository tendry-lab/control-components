/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "ocs_core/noncopyable.h"
#include "ocs_system/irebooter.h"

namespace ocs {
namespace system {

class DelayRebooter : public IRebooter, private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p delay to wait before the actual reboot is happened.
    //!  - @p rebooter to perform the actual reboot.
    DelayRebooter(TickType_t delay, IRebooter& rebooter);

    //! Wait extra delay and then reboot.
    void reboot() override;

private:
    const TickType_t delay_ { 0 };

    IRebooter& rebooter_;
};

} // namespace system
} // namespace ocs
