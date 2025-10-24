/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_system/reboot_task.h"

namespace ocs {
namespace system {

RebootTask::RebootTask(IRebooter& rebooter)
    : rebooter_(rebooter) {
}

status::StatusCode RebootTask::run() {
    rebooter_.reboot();
    return status::StatusCode::OK;
}

} // namespace system
} // namespace ocs
