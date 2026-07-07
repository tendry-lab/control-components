/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

namespace ocs {
namespace system {

class IDeviceId {
public:
    //! Destroy.
    virtual ~IDeviceId() = default;

    //! Return human-readable unique string device identifier.
    virtual const char* get_id() const = 0;
};

} // namespace system
} // namespace ocs
