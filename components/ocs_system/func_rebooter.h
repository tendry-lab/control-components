/*
 * SPDX-FileCopyrightText: 2026 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <functional>

#include "ocs_core/noncopyable.h"
#include "ocs_status/code.h"
#include "ocs_system/irebooter.h"

namespace ocs {
namespace system {

//! Functional rebooter.
class FuncRebooter : public IRebooter, private ocs::core::NonCopyable<> {
public:
    using Func = std::function<ocs::status::StatusCode(void)>;

    //! Initialize.
    //!
    //! @p func to be called on each reboot() call.
    explicit FuncRebooter(Func func);

    //! Run provided callback function.
    ocs::status::StatusCode reboot() override;

private:
    Func func_;
};

} // namespace system
} // namespace ocs
