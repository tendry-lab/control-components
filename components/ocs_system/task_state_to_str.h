/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_core/freertos.h"

namespace ocs {
namespace system {

//! Return human-readable FreeRTOS task state description.
const char* task_state_to_str(eTaskState state);

} // namespace system
} // namespace ocs
