/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_security/isha_engine.h"

namespace ocs {
namespace security {

class BasicShaEngine : public IShaEngine, public core::NonCopyable<BasicShaEngine> {
public:
    //! Destroy.
    virtual ~BasicShaEngine() = default;

    //! Initialize.
    explicit BasicShaEngine(IShaEngine::Algorithm algorithm);

    //! Return selected SHA algorithm.
    IShaEngine::Algorithm get_algorithm() const override;

protected:
    const IShaEngine::Algorithm algorithm_ { IShaEngine::Algorithm::SHA1 };
};

} // namespace security
} // namespace ocs
