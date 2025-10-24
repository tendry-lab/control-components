/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include <cstdio>
#include <cstring>

#include "freertos/FreeRTOSConfig.h"

#include "ocs_security/sha_to_hex_str.h"

namespace ocs {
namespace security {

sha_to_hex_str::sha_to_hex_str(const uint8_t* sha, unsigned size) {
    configASSERT(size <= max_hash_length_);

    memset(buf_, 0, sizeof(buf_));

    for (unsigned n = 0; n < size; ++n) {
        sprintf(buf_ + n * 2, "%02X", sha[n]);
    }
}

const char* sha_to_hex_str::c_str() const {
    return buf_;
}

} // namespace security
} // namespace ocs
