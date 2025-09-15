/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_system/device_id.h"

namespace ocs {
namespace system {

//! Various device characteristics.
class DeviceInfo : private core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p fw_name - unique FW name to distinguish one FW from another.
    //!  - @p fw_version - FW version in semver format.
    //!  - @p fw_description - a short user-friendly name that identifies the FW.
    //!  - @p product_name - unique product name to group related FWs.
    DeviceInfo(const char* fw_name,
               const char* fw_version,
               const char* fw_description,
               const char* product_name);

    const char* get_fw_name() const;
    const char* get_fw_version() const;
    const char* get_fw_description() const;
    const char* get_device_id() const;
    const char* get_product_name() const;

private:
    std::string fw_name_;
    std::string fw_version_;
    std::string fw_description_;
    std::string product_name_;
    DeviceID device_id_;
};

} // namespace system
} // namespace ocs
