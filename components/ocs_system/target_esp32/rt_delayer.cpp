/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "rom/ets_sys.h"

#include "ocs_system/target_esp32/rt_delayer.h"

namespace ocs {
namespace system {

status::StatusCode RtDelayer::delay(system::Time delay) {
    ets_delay_us(delay);
    return status::StatusCode::OK;
}

} // namespace system
} // namespace ocs
