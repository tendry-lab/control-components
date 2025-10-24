/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "esp_system.h"

namespace ocs {
namespace system {

using ResetReason = esp_reset_reason_t;

//! Return human-readable reset reason description.
const char* reset_reason_to_str(ResetReason reason);

} // namespace system
} // namespace ocs
