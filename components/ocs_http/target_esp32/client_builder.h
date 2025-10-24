/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>

#include "esp_http_client.h"

namespace ocs {
namespace http {

using ClientSharedPtr =
    std::shared_ptr<std::remove_pointer<esp_http_client_handle_t>::type>;

ClientSharedPtr make_client_shared(esp_http_client_config_t& config);

} // namespace http
} // namespace ocs
