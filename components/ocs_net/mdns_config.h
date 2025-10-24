/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_storage/iconfig.h"
#include "ocs_storage/istorage.h"
#include "ocs_system/device_info.h"

namespace ocs {
namespace net {

class MdnsConfig : public storage::IConfig, private core::NonCopyable<> {
public:
    // Maximum length of the mDNS hostname.
    static constexpr unsigned max_hostname_len = 31;

    //! Initialize.
    //!
    //! @params
    //!  - @p storage to persist mDNS configuration.
    //!  - @p device_info to use as a fallback for mDNS configuration.
    MdnsConfig(storage::IStorage& storage, const system::DeviceInfo& device_info);

    //! Reset mDNS configuration.
    //!
    //! @remarks
    //!  - Changes are applied on the component initialization.
    status::StatusCode reset() override;

    //! Return the configured mDNS hostname.
    const char* get_hostname() const;

    //! Update mDNS configuration.
    //!
    //! @remarks
    //!  - Changes are applied on the component initialization.
    status::StatusCode configure(const char* hostname);

private:
    static constexpr const char* hostname_key_ = "host";

    storage::IStorage& storage_;

    char hostname_[max_hostname_len + 1];
};

} // namespace net
} // namespace ocs
