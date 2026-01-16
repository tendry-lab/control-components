/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_status/code.h"

namespace ocs {
namespace system {

class IUpdateCommitter {
public:
    //! Destroy.
    virtual ~IUpdateCommitter() = default;

    //! Return true if recently installed firmware is waiting for verification.
    virtual bool require() = 0;

    //! Mark recently installed firmware as valid.
    virtual status::StatusCode commit() = 0;
};

} // namespace system
} // namespace ocs
