/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstring>

#include "ocs_core/static_event_group.h"

namespace ocs {
namespace core {

StaticEventGroup::StaticEventGroup() {
    memset(&buf_, 0, sizeof(buf_));

    group_ = xEventGroupCreateStatic(&buf_);
    configASSERT(group_);
}

EventGroupHandle_t StaticEventGroup::get() const {
    return group_;
}

} // namespace core
} // namespace ocs
