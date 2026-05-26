/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_system/device_id.h"
#include "ocs_system/iarena.h"

namespace ocs {
namespace system {

//! Various device characteristics.
class DeviceInfo : private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p arena to perform dynamic allocations.
    //!  - @p fw_name - unique FW name to distinguish one FW from another.
    //!  - @p fw_version - FW version in semver format.
    //!  - @p fw_description - a short user-friendly name that identifies the FW.
    //!  - @p product_name - unique product name to group related FWs.
    //!  - @p company_uuid - unique company string identifier.
    DeviceInfo(IArena& arena,
               const char* fw_name,
               const char* fw_version,
               const char* fw_description,
               const char* product_name,
               const char* company_uuid);

    const char* get_fw_name() const;
    const char* get_fw_version() const;
    const char* get_fw_description() const;
    const char* get_device_id() const;
    const char* get_product_name() const;

private:
    const std::string fw_name_;
    const std::string fw_version_;
    const std::string fw_description_;
    const std::string product_name_;
    const DeviceID device_id_;
};

} // namespace system
} // namespace ocs
