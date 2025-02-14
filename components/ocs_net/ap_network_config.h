/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_net/inetwork_config.h"
#include "ocs_storage/istorage.h"
#include "ocs_system/device_info.h"

namespace ocs {
namespace net {

//! WiFi AP configuration.
class ApNetworkConfig : public INetworkConfig, public core::NonCopyable<> {
public:
    //! Minimum password length.
    static constexpr unsigned min_password_len = 8;
    //! Maximum password length.
    static constexpr unsigned max_password_len = 63;

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
    //!
    //! @remarks
    //!  - Changes are applied on the component initialization.
    status::StatusCode configure(const char* password);

    //! Reset WiFi AP configuration.
    //!
    //! @remarks
    //!  - Changes are applied on the component initialization.
    status::StatusCode reset() override;

private:
    static constexpr const char* password_key_ = "net_ap_pswd";

    static constexpr unsigned max_ssid_len_ = 31;

    storage::IStorage& storage_;

    char ssid_[max_ssid_len_ + 1];
    char password_[max_password_len + 1];
};

} // namespace net
} // namespace ocs
