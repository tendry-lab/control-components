/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_system/iupdate_committer.h"

namespace ocs {
namespace system {

class UpdateCommitter : public IUpdateCommitter, private core::NonCopyable<> {
public:
    //! Return true if recently installed firmware is waiting for verification.
    bool require() override;

    //! Mark recently installed firmware as valid.
    status::StatusCode commit() override;
};

} // namespace system
} // namespace ocs
