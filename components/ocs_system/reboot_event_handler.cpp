/*
 * Copyright (c) 2025, Tendry Lab authors
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "ocs_system/reboot_event_handler.h"

namespace ocs {
namespace system {

RebootEventHandler::RebootEventHandler(IRebooter& rebooter)
    : rebooter_(rebooter) {
}

status::StatusCode RebootEventHandler::handle_event() {
    rebooter_.reboot();

    return status::StatusCode::OK;
}

} // namespace system
} // namespace ocs
