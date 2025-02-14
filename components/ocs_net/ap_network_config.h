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
namespace net {

class ApNetworkConfig : public core::NonCopyable<> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p storage to persist WiFi AP configuration.
    //!  - @p device_info to use as a fallback for WiFi AP configuration.
    ApNetworkConfig(storage::IStorage& storage, const system::DeviceInfo& device_info);

    //! Return WiFi AP SSID.
    const char* get_ssid() const;

    //! Return WiFi AP password.
    const char* get_password() const;

    //! Return WiFi AP channel.
    uint8_t get_channel() const;

    //! Return the maximum number of simultaneous WiFi STA connections to WiFi AP.
    uint8_t get_max_conn() const;

    //! Configure WiFi AP settings.
    status::StatusCode configure(const char* password);

    //! Reset WiFi AP configuration.
    status::StatusCode reset();

private:
    static constexpr const char* password_key_ = "net_ap_pswd";

    static constexpr unsigned max_ssid_size_ = 31;
    static constexpr unsigned max_password_size_ = 63;

    storage::IStorage& storage_;

    char ssid_[max_ssid_size_ + 1];
    char password_[max_password_size_ + 1];
};

} // namespace net
} // namespace ocs
