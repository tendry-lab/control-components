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

SystemPipeline::SystemPipeline(system::IArena& arena,
                               system::PlatformBuilder& platform_builder) {
    flash_initializer_ = ocs::system::make_unique_ptr<storage::FlashInitializer>(arena);
    configASSERT(flash_initializer_);

    clock_ = ocs::system::make_unique_ptr<system::Clock>(arena);
    configASSERT(clock_);

    fanout_reboot_handler_ =
        ocs::system::make_unique_ptr<system::FanoutRebootHandler>(arena);
    configASSERT(fanout_reboot_handler_);

    default_rebooter_ = platform_builder.make_rebooter(*fanout_reboot_handler_);
    configASSERT(default_rebooter_);

    rebooter_ = default_rebooter_.get();

    delay_rebooter_ = ocs::system::make_unique_ptr<system::DelayRebooter>(
        arena, *rebooter_, pdMS_TO_TICKS(500));

    rebooter_ = delay_rebooter_.get();

    configASSERT(rebooter_);

    device_info_ = ocs::system::make_unique_ptr<system::DeviceInfo>(
        arena, arena, CONFIG_OCS_CORE_FW_NAME, CONFIG_OCS_CORE_FW_VERSION,
        CONFIG_OCS_CORE_FW_DESCRIPTION, CONFIG_OCS_CORE_PRODUCT_NAME,
        CONFIG_OCS_CORE_COMPANY_UUID);
    configASSERT(device_info_);
}

system::IClock& SystemPipeline::get_clock() {
    return *clock_;
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
