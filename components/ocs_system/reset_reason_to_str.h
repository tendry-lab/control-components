/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
