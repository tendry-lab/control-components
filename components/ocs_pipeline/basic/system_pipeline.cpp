/*
 * SPDX-FileCopyrightText: 2025 Tendry Lab
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOSConfig.h"

#include "ocs_pipeline/basic/system_pipeline.h"
#include "ocs_status/code_to_str.h"
#include "ocs_system/delay_rebooter.h"
#include "ocs_system/target_esp32/clock.h"

namespace ocs {
namespace pipeline {
namespace basic {

SystemPipeline::SystemPipeline() {
    flash_initializer_ = std::make_unique<storage::FlashInitializer>();
    storage_builder_ = std::make_unique<storage::StorageBuilder>();
    clock_ = std::make_unique<system::Clock>();
    fanout_reboot_handler_ = std::make_unique<system::FanoutRebootHandler>();

    default_rebooter_ = system::PlatformBuilder::make_rebooter(*fanout_reboot_handler_);
    rebooter_ = default_rebooter_.get();

    delay_rebooter_ =
        std::make_unique<system::DelayRebooter>(*rebooter_, pdMS_TO_TICKS(500));

    rebooter_ = delay_rebooter_.get();

    configASSERT(rebooter_);

    device_info_ = std::make_unique<system::DeviceInfo>(
        CONFIG_OCS_CORE_FW_NAME, CONFIG_OCS_CORE_FW_VERSION,
        CONFIG_OCS_CORE_FW_DESCRIPTION, CONFIG_OCS_CORE_PRODUCT_NAME,
        CONFIG_OCS_CORE_COMPANY_UUID);
}

system::IClock& SystemPipeline::get_clock() {
    return *clock_;
}

storage::StorageBuilder& SystemPipeline::get_storage_builder() {
    return *storage_builder_;
}

system::IRebooter& SystemPipeline::get_rebooter() {
    return *rebooter_;
}

system::FanoutRebootHandler& SystemPipeline::get_reboot_handler() {
    return *fanout_reboot_handler_;
}

const system::DeviceInfo& SystemPipeline::get_device_info() const {
    return *device_info_;
}

} // namespace basic
} // namespace pipeline
} // namespace ocs
