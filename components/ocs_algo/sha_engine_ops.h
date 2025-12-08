/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "freertos/FreeRTOSConfig.h"

#include "ocs_security/isha_engine.h"

namespace ocs {
namespace algo {

struct ShaEngineOps {
    //! Return the maximum number of bytes the SHA algorithm can produce on the output.
    static constexpr size_t hash_lenght(security::IShaEngine::Algorithm algorithm) {
        switch (algorithm) {
        case security::IShaEngine::Algorithm::SHA1:
            return 20;

        case security::IShaEngine::Algorithm::SHA256:
            return 32;

        case security::IShaEngine::Algorithm::SHA512:
            return 64;
        }

        configASSERT(false);

        return 0;
    }
};

} // namespace algo
} // namespace ocs
