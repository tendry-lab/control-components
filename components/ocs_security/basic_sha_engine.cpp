/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ocs_security/basic_sha_engine.h"

namespace ocs {
namespace security {

BasicShaEngine::BasicShaEngine(IShaEngine::Algorithm algorithm)
    : algorithm_(algorithm) {
}

IShaEngine::Algorithm BasicShaEngine::get_algorithm() const {
    return algorithm_;
}

} // namespace security
} // namespace ocs
