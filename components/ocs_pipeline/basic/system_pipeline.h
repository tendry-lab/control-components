/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <memory>

#include "ocs_core/noncopyable.h"
#include "ocs_scheduler/idelay_estimator.h"
#include "ocs_scheduler/itask_scheduler.h"
#include "ocs_status/code.h"
#include "ocs_storage/storage_builder.h"
#include "ocs_storage/target_esp32/flash_initializer.h"
#include "ocs_system/device_info.h"
#include "ocs_system/fanout_reboot_handler.h"
#include "ocs_system/iclock.h"
#include "ocs_system/irebooter.h"
#include "ocs_system/platform_builder.h"

namespace ocs {
namespace pipeline {
namespace basic {

class SystemPipeline : private core::NonCopyable<> {
public:
    //! Initialize.
    SystemPipeline();

    const system::DeviceInfo& get_device_info() const;

    system::IClock& get_clock();
    storage::StorageBuilder& get_storage_builder();
    system::IRebooter& get_rebooter();
    system::FanoutRebootHandler& get_reboot_handler();

private:
    std::unique_ptr<storage::FlashInitializer> flash_initializer_;
    std::unique_ptr<storage::StorageBuilder> storage_builder_;

    std::unique_ptr<system::IClock> clock_;

    std::unique_ptr<system::FanoutRebootHandler> fanout_reboot_handler_;
    system::PlatformBuilder::IRebooterPtr default_rebooter_;
    std::unique_ptr<system::IRebooter> delay_rebooter_;
    system::IRebooter* rebooter_ { nullptr };

    std::unique_ptr<system::DeviceInfo> device_info_;
};

} // namespace basic
} // namespace pipeline
} // namespace ocs
