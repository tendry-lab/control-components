/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

namespace ocs {
namespace system {

class IRebooter {
public:
    //! Destroy.
    virtual ~IRebooter() = default;

    //! Restart the system.
    virtual void reboot() = 0;
};

} // namespace system
} // namespace ocs
