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

//! WiFi AP configuration.
class ApNetworkConfig : public storage::IConfig, private core::NonCopyable<> {
public:
    //! Minimum WiFi AP password length.
    static constexpr uint8_t min_password_len = 8;
    //! Maximum WiFi AP password length.
    static constexpr uint8_t max_password_len = 63;

    //! Minimum WiFi AP channel.
    static constexpr uint8_t min_channel = 1;

    //! Maximum WiFi AP channel.
    //!
    //! @remarks
    //!  - The US allows 1-11.
    //!  - The EU and most other countries allows 1-13.
    static constexpr uint8_t max_channel = 13;

    //! Minimum number of simultaneous STA connections.
    static constexpr uint8_t min_max_conn = 1;
    //! Maximum number of simultaneous STA connections.
    static constexpr uint8_t max_max_conn = 7;

    //! Initialize.
    //!
    //! @params
    //!  - @p storage to persist WiFi AP configuration.
    //!  - @p device_info to use as a fallback for WiFi AP configuration.
    ApNetworkConfig(storage::IStorage& storage, const system::DeviceInfo& device_info);

    //! Reset WiFi AP configuration.
    //!
    //! @remarks
    //!  - Changes are applied on the component initialization.
    status::StatusCode reset() override;

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
    //! @params
    //!  - @p channel - WiFi AP channel, see @min_channel, @max_channel.
    //!  - @p max_conn - maximum number of simultaneous STA connections,
    //!    see @min_max_conn, @max_max_conn.
    //!  - @p password - WiFi AP password, see @min_password_len, @max_password_len.
    //!
    //! @remarks
    //!  - Changes are applied on the component initialization.
    status::StatusCode configure(uint8_t channel, uint8_t max_conn, const char* password);

private:
    static constexpr const char* password_key_ = "password";
    static constexpr const char* channel_key_ = "channel";
    static constexpr const char* max_conn_key_ = "max_conn";

    static constexpr uint8_t max_ssid_len_ = 31;
    static constexpr uint8_t device_id_ssid_len_ = 12;

    static_assert(min_password_len <= device_id_ssid_len_,
                  "default password should be recognizable from the default SSID");

    storage::IStorage& storage_;

    char ssid_[max_ssid_len_ + 1];
    char password_[max_password_len + 1];
    uint8_t channel_ { 0 };
    uint8_t max_conn_ { 0 };
};

} // namespace net
} // namespace ocs
