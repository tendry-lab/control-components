/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_http/target_esp32/client_builder.h"

namespace ocs {
namespace http {

ClientSharedPtr make_client_shared(esp_http_client_config_t& config) {
    return ClientSharedPtr(esp_http_client_init(&config),
                           [](esp_http_client_handle_t client) {
                               if (client) {
                                   ESP_ERROR_CHECK(esp_http_client_close(client));
                                   ESP_ERROR_CHECK(esp_http_client_cleanup(client));
                               }
                           });
}

} // namespace http
} // namespace ocs
