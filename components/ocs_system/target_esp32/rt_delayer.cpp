/*
 * Copyright (c) 2024, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
