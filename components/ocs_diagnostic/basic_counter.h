/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_diagnostic/icounter.h"

namespace ocs {
namespace diagnostic {

class BasicCounter : public ICounter, public core::NonCopyable<BasicCounter> {
public:
    //! Initialize.
    //!
    //! @params
    //!  - @p id - counter identifier, 15 characters is a maximum length.
    explicit BasicCounter(const char* id);

    //! Destroy.
    virtual ~BasicCounter() = default;

    //! Return the underlying counter identifier.
    const char* id() const;

private:
    static constexpr uint8_t bufsize_ = 15;

    char buf_[bufsize_ + 1];
};

} // namespace diagnostic
} // namespace ocs
