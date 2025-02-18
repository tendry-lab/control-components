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

namespace ocs {
namespace net {

//! WiFi STA configuration.
class StaNetworkConfig : public INetworkConfig, public core::NonCopyable<> {
public:
    //! Minimum WiFi STA ssid length.
    static constexpr unsigned min_ssid_len = 1;
    //! Maximum WiFi STA ssid length.
    static constexpr unsigned max_ssid_len = 31;

    //! Minimum WiFi STA password length.
    static constexpr unsigned min_password_len = 8;
    //! Maximum WiFi STA password length.
    static constexpr unsigned max_password_len = 63;

    //! Minimum WiFi STA retry count.
    static constexpr unsigned min_max_retry_count = 1;
    //! Maximum WiFi STA retry count.
    static constexpr unsigned max_max_retry_count = 5;

    //! Initialize.
    //!
    //! @params
    //!  - @p storage to persist WiFi STA configuration.
    explicit StaNetworkConfig(storage::IStorage& storage);

    //! Return true if the WiFi STA configuration exists and is valid.
    bool valid() const;

    //! Return WiFi STA SSID.
    const char* get_ssid() const;

    //! Return WiFi STA password.
    const char* get_password() const;

    //! Return maximum number of times WiFi STA should try to connect to WiFi AP.
    uint8_t get_max_retry_count() const;

    //! Configure WiFi STA settings.
    //!
    //! @params
    //!  - @p max_retry_count - maximum number of attempts to connect to a WiFi AP,
    //!    see @min_max_retry_count, @max_max_retry_count.
    //!  - @p ssid - WiFi STA ssid, see @min_ssid_len, @max_ssid_len.
    //!  - @p password - WiFi STA password, see @min_password_len, @max_password_len.
    //!
    //! @remarks
    //!  - Changes are applied on the component initialization.
    status::StatusCode
    configure(uint8_t max_retry_count, const char* ssid, const char* password);

    //! Reset WiFi STA configuration.
    //!
    //! @remarks
    //!  - Changes are applied on the component initialization.
    status::StatusCode reset() override;

private:
    static constexpr uint8_t default_max_retry_count_ = 5;
    static constexpr const char* max_retry_count_key_ = "retry";
    static constexpr const char* password_key_ = "password";
    static constexpr const char* ssid_key_ = "ssid";

    storage::IStorage& storage_;

    uint8_t max_retry_count_ { 0 };
    char ssid_[max_ssid_len + 1];
    char password_[max_password_len + 1];
};

} // namespace net
} // namespace ocs
