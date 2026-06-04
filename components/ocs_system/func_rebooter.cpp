/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_system/func_rebooter.h"

namespace ocs {
namespace system {

FuncRebooter::FuncRebooter(Func func)
    : func_(func) {
}

ocs::status::StatusCode FuncRebooter::reboot() {
    return func_();
}

} // namespace system
} // namespace ocs
