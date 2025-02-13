/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_storage/istorage.h"
#include "ocs_system/device_info.h"

namespace ocs {
namespace pipeline {
namespace config {

class MdnsConfig : public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p storage to persist mDNS configuration.
    //!  - @p device_info to use as a fallback for mDNS configuration.
    MdnsConfig(storage::IStorage& storage, const system::DeviceInfo& device_info);

    //! Return the configured mDNS hostname.
    const char* get_hostname() const;

    //! Update mDNS configuration.
    status::StatusCode configure(const char* hostname);

    //! Reset mDNS configuration.
    status::StatusCode reset();

private:
    static constexpr const char* hostname_key_ = "host";
    static constexpr unsigned max_hostname_size_ = 31;

    storage::IStorage& storage_;

    char hostname_[max_hostname_size_ + 1];
};

} // namespace config
} // namespace pipeline
} // namespace ocs
