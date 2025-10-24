/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "esp_timer.h"

#include "ocs_system/target_esp32/default_clock.h"

namespace ocs {
namespace system {

system::Time DefaultClock::now() {
    return esp_timer_get_time();
}

} // namespace system
} // namespace ocs
