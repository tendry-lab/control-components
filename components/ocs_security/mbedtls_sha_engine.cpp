/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbedtls/sha1.h"
#include "mbedtls/sha256.h"

#if SOC_SHA_SUPPORT_SHA512
#include "mbedtls/sha512.h"
#endif // SOC_SHA_SUPPORT_SHA512

#include "ocs_security/mbedtls_sha_engine.h"

namespace ocs {
namespace security {

MbedTlsShaEngine::MbedTlsShaEngine(IShaEngine::Algorithm algorithm)
    : BasicShaEngine(algorithm) {
}

status::StatusCode
MbedTlsShaEngine::generate(uint8_t* buf, const uint8_t* src, size_t size) {
    int result = 0;

    switch (algorithm_) {
    case IShaEngine::Algorithm::SHA1:
        result = mbedtls_sha1(src, size, buf);
        break;

    case IShaEngine::Algorithm::SHA256:
        result = mbedtls_sha256(src, size, buf, 0);
        break;

#if SOC_SHA_SUPPORT_SHA512
    case IShaEngine::Algorithm::SHA512:
        result = mbedtls_sha512(src, size, buf, 0);
        break;
#endif // SOC_SHA_SUPPORT_SHA512

    default:
        return status::StatusCode::InvalidArg;
    }

    return result == 0 ? status::StatusCode::OK : status::StatusCode::Error;
}

} // namespace security
} // namespace ocs
