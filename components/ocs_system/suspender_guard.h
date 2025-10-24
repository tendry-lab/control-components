/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_system/isuspender.h"

namespace ocs {
namespace system {

class SuspenderGuard : private core::NonCopyable<> {
public:
    //! Suspend on initialization.
    explicit SuspenderGuard(ISuspender& suspender);

    //! Resume on de-initialization.
    ~SuspenderGuard();

private:
    ISuspender& suspender_;
};

} // namespace system
} // namespace ocs
