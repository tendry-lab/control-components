/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

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
    explicit Rebooter(IRebootHandler& handler);

    //! Restart the board.
    void reboot() override;

private:
    IRebootHandler& handler_;
};

} // namespace system
} // namespace ocs
