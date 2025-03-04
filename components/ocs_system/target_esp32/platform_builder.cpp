/*
 * Copyright (c) 2025, Open Control Systems authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_system/platform_builder.h"
#include "ocs_system/target_esp32/rt_delayer.h"

namespace ocs {
namespace system {

PlatformBuilder::IRtDelayerPtr PlatformBuilder::make_rt_delayer() {
    return IRtDelayerPtr(new (std::nothrow) RtDelayer());
}

} // namespace system
} // namespace ocs
