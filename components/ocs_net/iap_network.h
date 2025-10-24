/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <cstdint>

namespace ocs {
namespace net {

class IApNetwork {
public:
    struct Info {
        //! WiFi channel.
        uint8_t channel { 0 };

        //! Current number of STA connections to the AP.
        uint8_t cur_connection { 0 };

        //! Maximum number of simultaneous STA connections to the AP.
        uint8_t max_connection { 0 };
    };

    //! Destroy.
    virtual ~IApNetwork() = default;

    //! Return various network characteristics.
    virtual Info get_info() = 0;
};

} // namespace net
} // namespace ocs
