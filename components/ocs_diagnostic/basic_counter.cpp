/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <algorithm>
#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_diagnostic/basic_counter.h"

namespace ocs {
namespace diagnostic {

BasicCounter::BasicCounter(const char* id) {
    configASSERT(id);
    configASSERT(strlen(id));

    memset(buf_, 0, sizeof(buf_));
    strncpy(buf_, id, std::min(bufsize_, strlen(id)));
}

const char* BasicCounter::id() const {
    return buf_;
}

} // namespace diagnostic
} // namespace ocs
