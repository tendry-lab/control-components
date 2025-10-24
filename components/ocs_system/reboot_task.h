/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "ocs_core/noncopyable.h"
#include "ocs_scheduler/itask.h"
#include "ocs_system/irebooter.h"

namespace ocs {
namespace system {

class RebootTask : public scheduler::ITask, private core::NonCopyable<> {
public:
    //! Initialize.
    explicit RebootTask(IRebooter& rebooter);

    //! Initiate a reboot process.
    status::StatusCode run() override;

private:
    IRebooter& rebooter_;
};

} // namespace system
} // namespace ocs
