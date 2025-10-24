/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>

#include "esp_netif.h"

namespace ocs {
namespace net {

using NetifSharedPtr = std::shared_ptr<esp_netif_t>;

NetifSharedPtr make_netif_shared(esp_netif_t* netif);

} // namespace net
} // namespace ocs
