/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <string>

#include "ocs_core/noncopyable.h"

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
    const char* get_product_name() const;

private:
    const std::string fw_name_;
    const std::string fw_version_;
    const std::string fw_description_;
    const std::string product_name_;
};

} // namespace system
} // namespace ocs
