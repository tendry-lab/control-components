/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstring>

#include "esp_wifi.h"

#include "ocs_net/target_esp32/netif_builder.h"

namespace ocs {
namespace net {

NetifSharedPtr make_netif_shared(esp_netif_t* netif) {
    return NetifSharedPtr(netif, [](esp_netif_t* netif) {
        if (netif) {
            esp_netif_action_disconnected(netif, nullptr, 0, nullptr);

            if (strcmp(esp_netif_get_desc(netif), "sta") == 0) {
                esp_netif_action_stop(netif, nullptr, 0, nullptr);

                ESP_ERROR_CHECK(esp_wifi_clear_default_wifi_driver_and_handlers(netif));
            }

            esp_netif_destroy(netif);
        }
    });
}

} // namespace net
} // namespace ocs
