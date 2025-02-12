/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_storage/storage_builder.h"
#include "ocs_system/device_info.h"

namespace ocs {
namespace pipeline {
namespace config {

class MdnsConfig : public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p device_info to use as a fallback for mDNS configuration.
    //!  - @p storage_builder to persist mDNS configuration.
    MdnsConfig(const system::DeviceInfo& device_info,
               storage::StorageBuilder& storage_builder);

    //! Return the configured mDNS hostname.
    const char* get_hostname() const;

    //! Update mDNS configuration.
    status::StatusCode configure(const char* hostname);

private:
    static constexpr const char* hostname_key_ = "host";

    static constexpr unsigned max_hostname_size_ = 31;

    storage::StorageBuilder::IStoragePtr storage_;

    char hostname_[max_hostname_size_ + 1];
};

} // namespace config
} // namespace pipeline
} // namespace ocs
