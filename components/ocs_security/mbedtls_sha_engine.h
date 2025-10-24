/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_security/basic_sha_engine.h"

namespace ocs {
namespace security {

class MbedTlsShaEngine : public BasicShaEngine, private core::NonCopyable<> {
public:
    //! Initialize.
    explicit MbedTlsShaEngine(IShaEngine::Algorithm algorithm);

    //! Generate SHA by using the mbedTLS library.
    status::StatusCode generate(uint8_t* buf, const uint8_t* src, unsigned size) override;
};

} // namespace security
} // namespace ocs
