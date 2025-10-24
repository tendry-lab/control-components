/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

namespace ocs {
namespace system {

class IRebootHandler {
public:
    //! Destroy.
    virtual ~IRebootHandler() = default;

    //! Handle reboot event.
    virtual void handle_reboot() = 0;
};

} // namespace system
} // namespace ocs
