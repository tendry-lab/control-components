/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <functional>
#include <memory>

#include "ocs_core/future.h"
#include "ocs_status/code.h"

namespace ocs {
namespace scheduler {

class IFuncScheduler {
public:
    using FuturePtr = std::shared_ptr<core::Future>;
    using Func = std::function<status::StatusCode()>;

    //! Destroy.
    virtual ~IFuncScheduler() = default;

    //! Add @p func to be executed asynchronously.
    //!
    //! @remarks
    //!  It is safe to call scheduler functions in @p func.
    //!
    //! @return
    //!  Future to wait for the @p func result, nullptr if @p func can't be scheduled.
    virtual FuturePtr add(Func func) = 0;
};

} // namespace scheduler
} // namespace ocs
