/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <vector>

#include "ocs_core/noncopyable.h"
#include "ocs_system/ireboot_handler.h"

namespace ocs {
namespace system {

class FanoutRebootHandler : public IRebootHandler, private core::NonCopyable<> {
public:
    //! Deliver reboot event to the underlying handlers.
    void handle_reboot() override;

    //! Add handler to be notified when the reboot event is happened.
    void add(IRebootHandler& handler);

private:
    std::vector<IRebootHandler*> handlers_;
};

} // namespace system
} // namespace ocs
