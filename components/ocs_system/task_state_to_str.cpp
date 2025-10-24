/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_system/task_state_to_str.h"

namespace ocs {
namespace system {

const char* task_state_to_str(eTaskState state) {
    switch (state) {
    case eRunning:
        return "Running";

    case eReady:
        return "Ready";

    case eBlocked:
        return "Blocked";

    case eSuspended:
        return "Suspended";

    case eDeleted:
        return "Deleted";

    case eInvalid:
        break;

    default:
        break;
    }

    return "<none>";
}

} // namespace system
} // namespace ocs
